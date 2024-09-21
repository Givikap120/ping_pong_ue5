// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Ball.generated.h"

UCLASS()
class PING_PONG_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* BallBody;

	UPROPERTY(EditDefaultsOnly)
	float BallSpeed = 2000;

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	FVector InitialPosition;
	FVector InitialDirection;

	FVector BallVelocity;
	bool isReflecting;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ResetBall();
};
