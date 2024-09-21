// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraActor.h"
#include "Ball.h"
#include "PaddlePawn.h"

#include "PongGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PING_PONG_API APongGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	ACameraActor* Camera;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	ABall* Ball;

	UPROPERTY(BlueprintReadWrite)
	int Player0_Score;

	UPROPERTY(BlueprintReadWrite)
	int Player1_Score;

	UPROPERTY(BlueprintReadWrite)
	TArray<APlayerController*> Controllers;

	UFUNCTION(BlueprintNativeEvent)
	void HandleGoalScored(int PlayerId);
public:

	UPROPERTY(BlueprintReadOnly)
	TArray<APaddlePawn*> PaddlePawns;

	UFUNCTION(BlueprintCallable)
	ACameraActor* GetCamera();

	UFUNCTION(BlueprintCallable)
	void HandleNewPlayer(APlayerController* NewPlayer);
};
