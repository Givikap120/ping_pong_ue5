// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "InputMappingContext.h"

#include "PaddlePawn.generated.h"

UCLASS()
class PING_PONG_API APaddlePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaddlePawn();

private:
	float velocity;
	FVector startingPosition;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PaddleBody;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionBox;

	void ActivateEnchancedInput();

	UFUNCTION(Server, Reliable)
	void ServerGetCamera();

	UFUNCTION(Client, Reliable)
	void ClientSetCamera(APlayerController* activeController, ACameraActor* camera);

	UFUNCTION(Server, Reliable)
	void ServerMove(float newVelocity);

	void Move(const FInputActionValue& Action);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ActionMove;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 1000;

	UPROPERTY(EditDefaultsOnly)
	float RangeForMovement = 900;

	UPROPERTY(EditDefaultsOnly)
	int PlayerID;
};
