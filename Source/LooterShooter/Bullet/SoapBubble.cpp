#include "SoapBubble.h"
#include "../Character/PlayerCharacter.h"

ASoapBubble::ASoapBubble()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ASoapBubble::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> Components;
	GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("UStaticMeshComponent")))
        {
            SkeletalMeshComponent = Cast<UStaticMeshComponent>(Component);
        }
		if (Component && Component->ComponentHasTag(TEXT("CollisionSphere")))
		{
			CollisionSphere = Cast<USphereComponent>(Component);
		}
    }

	if (CollisionSphere) {
		CollisionSphere->OnComponentHit.AddDynamic(this, &ASoapBubble::OnHit);
	}
}

void ASoapBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoapBubble::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
			if (Player)
			{
				Player->bIsBubbleMode = true;
			}
		}

		Destroy();
	}
}