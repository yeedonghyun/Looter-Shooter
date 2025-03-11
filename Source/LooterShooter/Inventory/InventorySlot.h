// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "../Inventory/DragDropSlot.h"
#include "InventorySlot.generated.h"


UCLASS()
class LOOTERSHOOTER_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetVisible();
	void SetColor(float r, float g, float b, float a);
public:

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Item;
		 
	UPROPERTY(meta = (BindWidget))
	UImage* Default;

	int idx;
};

