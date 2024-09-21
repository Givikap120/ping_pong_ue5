// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddlePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PongGameMode.h"

// Sets default values
APaddlePawn::APaddlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	PaddleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleBody"));
	RootComponent = PaddleBody;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(PaddleBody);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void APaddlePawn::BeginPlay()
{
	Super::BeginPlay();

	startingPosition = GetActorLocation();

	FBox LocalBoundingBox = PaddleBody->GetStaticMesh()->GetBoundingBox();
	FVector BoxExtent = LocalBoundingBox.GetExtent();
	CollisionBox->SetBoxExtent(BoxExtent);

	ActivateEnchancedInput();
	ServerGetCamera();
}

void APaddlePawn::Move(const FInputActionValue& action)
{
	float newVelocity = action.Get<float>();
	velocity = newVelocity;

	if (!HasAuthority())
	{
		ServerMove(action.Get<float>());
	}
}

void APaddlePawn::ServerMove_Implementation(float newVelocity)
{
	velocity = newVelocity;
}

void APaddlePawn::ServerGetCamera_Implementation()
{
	auto controller = Cast<APlayerController>(GetController());
	if (controller == NULL)
		return;

	auto gamemode = Cast<APongGameMode>(GetWorld()->GetAuthGameMode());
	if (gamemode == NULL)
		return;

	auto camera = gamemode->GetCamera();
	if (camera == NULL)
		return;

	ClientSetCamera(controller, camera);
}

void APaddlePawn::ClientSetCamera_Implementation(APlayerController* controller, ACameraActor* camera)
{
	controller->SetViewTargetWithBlend(camera);
}

void APaddlePawn::ActivateEnchancedInput()
{
	auto controller = Cast<APlayerController>(GetController());
	if (controller == NULL)
		return;

	auto player = controller->GetLocalPlayer();
	if (player == NULL)
		return;

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(player);
	if (subsystem == NULL)
		return;

	subsystem->AddMappingContext(InputMappingContext, 0);
}

// Called every frame
void APaddlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(0, velocity * MaxSpeed * DeltaTime, 0));

	FVector currentPosition = GetActorLocation();
	currentPosition.Y = FMath::Clamp(currentPosition.Y, startingPosition.Y - RangeForMovement, startingPosition.Y + RangeForMovement);
	SetActorLocation(currentPosition);
}

// Called to bind functionality to input
void APaddlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnchancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnchancedInput->BindAction(ActionMove, ETriggerEvent::Triggered, this, &APaddlePawn::Move);
	EnchancedInput->BindAction(ActionMove, ETriggerEvent::Completed, this, &APaddlePawn::Move);
}

