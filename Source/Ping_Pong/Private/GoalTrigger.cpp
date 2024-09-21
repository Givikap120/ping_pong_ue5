// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalTrigger.h"
#include "Ball.h"

// Sets default values
AGoalTrigger::AGoalTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GoalZone = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalZone"));
    RootComponent = GoalZone;

    GoalZone->OnComponentBeginOverlap.AddDynamic(this, &AGoalTrigger::OnOverlapBegin);
}

void AGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA<ABall>())
    {
        OnGoalScored.Broadcast(PlayerID);
    }
}

