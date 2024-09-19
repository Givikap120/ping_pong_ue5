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

    GoalZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GoalZone->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
    GoalZone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    GoalZone->OnComponentBeginOverlap.AddDynamic(this, &AGoalTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGoalTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ABall::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Goal Scored! PlayerID = %d"), PlayerID);
        OnGoalScored.Broadcast(PlayerID);
    }
}

