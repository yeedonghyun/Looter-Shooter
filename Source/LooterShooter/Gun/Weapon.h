// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class LOOTERSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USkeletalMeshComponent* SkeletalMeshComponent;
	USceneComponent* EndPoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetSkeletalMeshComponent();
	FVector GetEndPointLocation();
};
