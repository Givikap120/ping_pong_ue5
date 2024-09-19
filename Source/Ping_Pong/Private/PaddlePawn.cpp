// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddlePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APaddlePawn::APaddlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaddleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleBody"));
	RootComponent = PaddleBody;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(PaddleBody);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetNotifyRigidBodyCollision(true);

	//CollisionBox->SetSimulatePhysics(true);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
}

void APaddlePawn::Move(const FInputActionValue& action)
{
	AddMovementInput(FVector(0, 1, 0), action.Get<float>());
}

// Called when the game starts or when spawned
void APaddlePawn::BeginPlay()
{
	Super::BeginPlay();

	FBox LocalBoundingBox = PaddleBody->GetStaticMesh()->GetBoundingBox();
	FVector BoxExtent = LocalBoundingBox.GetExtent();
	CollisionBox->SetBoxExtent(BoxExtent);

	APlayerController* controller = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer());
	subsystem->AddMappingContext(InputMappingContext, 0);
}

// Called every frame
void APaddlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APaddlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnchancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnchancedInput->BindAction(ActionMove, ETriggerEvent::Triggered, this, &APaddlePawn::Move);
}

