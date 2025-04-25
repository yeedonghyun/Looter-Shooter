// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/InventorySlot.h"
#include "LooterShooter/Item/ItemBase.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Save/SaveManager.h"
#include "Tooltip.h"
#include "Components/Button.h"

#include "Delegates/DelegateCombinations.h"

#include "InventoryBase.generated.h"



UCLASS()
class LOOTERSHOOTER_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

};
