#include "Weapon.h"
#include "PhysicsEngine/PhysicsAsset.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
    TArray<UActorComponent*> Components;
    GetComponents(Components);
	bIsDrroped = false;

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("SkeletalMeshComponent")))
        {
            SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component);
        }

		else if (Component && Component->ComponentHasTag(TEXT("EndPointActor")))
		{
			EndPoint = Cast<USceneComponent>(Component);
		}

		else if (Component && Component->ComponentHasTag(TEXT("FIreEeffect")))
		{
			MuzzleFlash = Cast<UParticleSystemComponent>(Component);
		}

		else if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Component))
		{
			if (ChildActorComp->ComponentHasTag(TEXT("Gun")))
			{
				GunItem = Cast<AItem_Gun>(ChildActorComp->GetChildActor());
			}
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

void AWeapon::SpawnMuzzleFlash()
{
	if (MuzzleFlash && SkeletalMeshComponent)
	{
		MuzzleFlash->ActivateSystem(true);
	}
}

void AWeapon::OnPhysicsSimulate()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (UPhysicsAsset* PhysicsAsset = LoadObject<UPhysicsAsset>(nullptr, TEXT("/Script/Engine.PhysicsAsset'/Game/Assets/Guns/AssaultRifle/Meshes/PA_SK_AssaultRifle.PA_SK_AssaultRifle'")))
	{
		SkeletalMeshComponent->SetPhysicsAsset(PhysicsAsset, false);

		SkeletalMeshComponent->SetSimulatePhysics(true);
		SkeletalMeshComponent->WakeAllRigidBodies();

		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}
}

void AWeapon::Freeze()
{
	bIsDrroped = true;

	SkeletalMeshComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector, false);
	SkeletalMeshComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

	SkeletalMeshComponent->SetSimulatePhysics(false);

	SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMeshComponent->bPauseAnims = true;
	SkeletalMeshComponent->SetComponentTickEnabled(false);
}