// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Wall.h"
#include "PaddlePawn.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	BallBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallBody"));
	RootComponent = BallBody;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(BallBody);

	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionCapsule->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	InitialPosition = GetActorLocation();
	InitialDirection = GetActorForwardVector();

	FBox LocalBoundingBox = BallBody->GetStaticMesh()->GetBoundingBox();
	FVector BoxExtent = LocalBoundingBox.GetExtent();
	float Radius = FMath::Max(BoxExtent.X, BoxExtent.Y);
	float HalfHeight = BoxExtent.Z;

	CollisionCapsule->SetCapsuleSize(Radius, HalfHeight);
}

void ABall::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isReflecting)
		return;

	isReflecting = true;

	if (OtherActor && OtherActor->IsA<AWall>())
	{
		auto wall = Cast<AWall>(OtherActor);
		float rotation = FMath::DegreesToRadians(wall->GetRotation());

		FVector Normal = FVector(FMath::Cos(rotation), FMath::Sin(rotation), 0.0f).GetSafeNormal();

		float DotProduct = FVector::DotProduct(BallVelocity, Normal);

		BallVelocity = BallVelocity - 2 * DotProduct * Normal;
	}
	else if(OtherActor && OtherActor->IsA<APaddlePawn>())
	{
		BallVelocity.X = -BallVelocity.X;
	}
}

void ABall::ResetBall()
{
	float RandomAngle = FMath::RandRange(-PI / 4, PI / 4);

	FRotator Rotation = FRotator(0.0f, FMath::RadiansToDegrees(RandomAngle), 0.0f);
	FVector LaunchDirection = Rotation.RotateVector(InitialDirection);

	LaunchDirection.Normalize();

	BallVelocity = LaunchDirection * BallSpeed;

	SetActorLocation(InitialPosition);
	ForceNetUpdate();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation() + BallVelocity * DeltaTime;
	SetActorLocation(NewLocation);
	isReflecting = false;
}

