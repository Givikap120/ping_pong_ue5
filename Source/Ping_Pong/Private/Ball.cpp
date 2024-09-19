// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallBody"));
	RootComponent = BallBody;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->bShouldBounce = true;
	MovementComponent->Bounciness = 1.0f;
	MovementComponent->Friction = 0.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	/*CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(BallBody);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetNotifyRigidBodyCollision(true);

	CollisionBox->SetSimulatePhysics(true);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnBeingOverlap);

	BallSpeed = 1000.0f;
	BallVelocity = FVector::ZeroVector;*/
}

void ABall::OnBeingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Not working because SweepResult isn't populated
	BallVelocity = FVector::VectorPlaneProject(BallVelocity, SweepResult.Normal);
	BallVelocity = BallVelocity.MirrorByVector(SweepResult.Normal);
}

void ABall::ResetBall()
{
	float RandomAngle = FMath::RandRange(-PI / 4, PI / 4);

	FRotator Rotation = FRotator(0.0f, FMath::RadiansToDegrees(RandomAngle), 0.0f);
	FVector LaunchDirection = Rotation.RotateVector(InitialDirection);

	LaunchDirection.Normalize();
	BallVelocity = LaunchDirection * MovementComponent->MaxSpeed;

	//BallVelocity = LaunchDirection * BallSpeed;

	// Use movement component for now
	MovementComponent->Velocity = BallVelocity;

	SetActorLocation(InitialPosition);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = GetActorLocation();
	InitialDirection = GetActorForwardVector();
	
	/*FBox LocalBoundingBox = BallBody->GetStaticMesh()->GetBoundingBox();
	FVector BoxExtent = LocalBoundingBox.GetExtent();
	CollisionBox->SetBoxExtent(BoxExtent);*/

	ResetBall();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector NewLocation = GetActorLocation() + BallVelocity * DeltaTime;
	SetActorLocation(NewLocation);*/
}

