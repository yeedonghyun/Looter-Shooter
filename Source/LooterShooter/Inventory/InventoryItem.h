// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemBase : uint8
{
	WEAPON UMETA(DisplayName = "Weapon"),
	AMMO UMETA(DisplayName = "Ammo"),
	HEALING UMETA(DisplayName = "Healing")
};

UCLASS()
class LOOTERSHOOTER_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	

public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		EInventoryItemBase inventoryItemBase;
};
