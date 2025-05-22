// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Item/Item_Gun.h"
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

	UParticleSystemComponent* MuzzleFlash;


public:	
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetSkeletalMeshComponent();

	UFUNCTION(BlueprintCallable)
	FVector GetEndPointLocation();

	UFUNCTION(BlueprintCallable)
	void SpawnMuzzleFlash();

	bool bIsDrroped;
	AItem_Gun* GunItem;

public:
	UFUNCTION(BlueprintCallable)
	void Freeze();

	UFUNCTION(BlueprintCallable)
	void OnPhysicsSimulate();
};