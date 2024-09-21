// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "GoalTrigger.h"

void APongGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundPaddlePawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaddlePawn::StaticClass(), FoundPaddlePawns);
    for (AActor* Actor : FoundPaddlePawns)
    {
        APaddlePawn* CastedActor = Cast<APaddlePawn>(Actor);
        if (CastedActor)
        {
            PaddlePawns.Add(CastedActor);
        }
    }

    PaddlePawns.Sort([](const APaddlePawn& A, const APaddlePawn& B)
    {
        return A.PlayerID < B.PlayerID;
    });

    AActor* FoundBall = UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass());
    Ball = Cast<ABall>(FoundBall);

    TArray<AActor*> FoundGoalTriggers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalTrigger::StaticClass(), FoundGoalTriggers);
    for (AActor* Actor : FoundGoalTriggers)
    {
        AGoalTrigger* GoalTrigger = Cast<AGoalTrigger>(Actor);
        GoalTrigger->OnGoalScored.AddDynamic(this, &APongGameMode::HandleGoalScored);
    }


}

void APongGameMode::HandleGoalScored_Implementation(int playerId)
{
    if (playerId < 0 || playerId >= 2)
        return;

    if (playerId == 0) Player0_Score++;
    else Player1_Score++;

    Ball->ResetBall();
}

ACameraActor* APongGameMode::GetCamera()
{
    if (!Camera)
    {
        TArray<AActor*> FoundCameras;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PlayerCamera"), FoundCameras);

        if (FoundCameras.Num() > 0)
        {
            Camera = Cast<ACameraActor>(FoundCameras[0]);
        }
    }

    return Camera;
}

void APongGameMode::HandleNewPlayer(APlayerController* NewPlayer)
{
    int currentPlayerId = Controllers.Num();
    if (currentPlayerId >= PaddlePawns.Num())
        return;

    auto currentPaddle = PaddlePawns[currentPlayerId];
    NewPlayer->Possess(currentPaddle);
    Controllers.Add(NewPlayer);
}
