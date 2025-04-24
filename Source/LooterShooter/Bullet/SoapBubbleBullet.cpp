#include "SoapBubbleBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASoapBubbleBullet::ASoapBubbleBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->InitSphereRadius(15.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	CollisionSphere->OnComponentHit.AddDynamic(this, &ASoapBubbleBullet::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; 
}

void ASoapBubbleBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ASoapBubbleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoapBubbleBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoapBubbleBullet hit: %s"), *OtherActor->GetName());

		Destroy();
	}
}