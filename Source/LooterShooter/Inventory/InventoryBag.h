// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/InventoryItem.h"

#include "InventoryBag.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UInventoryBag : public UInventoryItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 bagSize;

};
