#include "SoapBubbleBullet.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/EnemyCharacter.h"
#include "../Character/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoapBubbleBullet::ASoapBubbleBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 5.0f;
	damage = 1000;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionMesh"));
	CollisionSphere->InitSphereRadius(5.0f);
	RootComponent = CollisionSphere;
}

void ASoapBubbleBullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentHit.AddDynamic(this, &ASoapBubbleBullet::OnHit);
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
		if (OtherActor->ActorHasTag("Enemy"))
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
			if (Enemy)
			{
				Enemy->ApplyDamage(damage);
			}
		}

		Destroy();
	}
}