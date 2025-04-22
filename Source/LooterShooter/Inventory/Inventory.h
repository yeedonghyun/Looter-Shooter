// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/EditableTextBox.h"

#include "../Item/ItemData.h"

#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UInventory : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InventoryName;

	UPROPERTY(meta = (BindWidget))
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* Name;


	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Grid;

	
};
