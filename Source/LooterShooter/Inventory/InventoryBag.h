// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/InventoryItem.h"

#include "InventoryBag.generated.h"

//USTRUCT(BlueprintType)
//struct FSavedItem
//{
//	GENERATED_BODY()
//
//public:
//	int x;
//	int y;
//	UInventoryItem* item;
//};


UCLASS()
class LOOTERSHOOTER_API UInventoryBag : public UInventoryItem
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	//int32 bagHeight;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	//int32 bagWidth;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	//TArray<FSavedItem> savedItems;

};



