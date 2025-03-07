// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/Image.h"


#include "ItemBase.generated.h"

UCLASS()
class LOOTERSHOOTER_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	enum ItemType
	{
		WEAPON,
		AMMO,
		HEALING
	};

	ItemType Type;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FString Name;
	int Value;
	int Weight;

	UStaticMeshComponent* StaticMeshComponent;
	UImage* Image;

public:
	FString GetName();
	int GetValue();
	int GetWeight();
	UStaticMeshComponent* GeStaticMeshComponent();
	UImage* GetImage();
};
