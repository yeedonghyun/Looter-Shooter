#include "Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("SkeletalMeshComponent")))
        {
            SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component);
        }

		if (Component && Component->ComponentHasTag(TEXT("EndPointActor")))
		{
			EndPoint = Cast<USceneComponent>(Component);
		}
    }
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USkeletalMeshComponent* AWeapon::GetSkeletalMeshComponent()
{
    return SkeletalMeshComponent;
}

FVector AWeapon::GetEndPointLocation()
{
	if (!EndPoint)
	{
		return FVector::ZeroVector;
	}

	return EndPoint->GetComponentLocation();
}