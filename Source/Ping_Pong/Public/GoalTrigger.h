// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "GoalTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalScored, int32, PlayerID);

UCLASS()
class PING_PONG_API AGoalTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalTrigger();

private:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Box component to represent the goal area
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* GoalZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerID = 0;

	UPROPERTY(BlueprintAssignable)
	FOnGoalScored OnGoalScored;
};
