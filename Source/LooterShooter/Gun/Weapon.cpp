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
            break;
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
