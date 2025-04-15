// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"
#include "../Inventory/InventoryBase.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Item/Item_bag.h"
#include "Components/TextBlock.h"
#include "../Inventory/Tooltip.h"

#include "PlayerInventoryWidget.generated.h"


DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FDropInventoryItem, FString)

UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UInventoryBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitWidget();
	void LoadInventoryData();


	int FindEmptySlot(TArray<UInventorySlot*>& SlotArray);
	void AddItemEmptySlot(AItemBase* AimedItem);
	void CreateWorldInventory(AItemBase* AimedItem);
	void DeleteWorldInventory();

	void ToggleInventory(bool bOpen);
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData);


protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* EquipInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* WorldInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* ArmorSlot;


	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerInventory;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* WorldInventory;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* EquipInventory;

	


public:

	TArray<UInventorySlot*> PlayerInventoryArray;
	TArray<UInventorySlot*> WorldInventoryArray;
	TArray<UInventorySlot*> EquipInventoryArray;

	int32 InventoryRowSize;
	int32 InventoryColSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bEquipInventory;



	bool bOtherInventory;



	AItem_bag* Bag;

	FDropInventoryItem OnDropRequested;

};
