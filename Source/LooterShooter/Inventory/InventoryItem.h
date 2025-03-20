// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Item/ItemBase.h"
#include "InventoryItem.generated.h"


UCLASS()
class LOOTERSHOOTER_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, Category = "Slot")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 Value;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	EItemType Type;
};
