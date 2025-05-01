// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon.generated.h"

UCLASS()
class LOOTERSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

	USkeletalMeshComponent* SkeletalMeshComponent;
	USceneComponent* EndPoint;

public:	
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetSkeletalMeshComponent();

	UFUNCTION(BlueprintCallable)
	FVector GetEndPointLocation();

	UParticleSystemComponent* MuzzleFlash;

	UFUNCTION(BlueprintCallable)
	void SpawnMuzzleFlash();
};
