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


	void AddItemEmptySlot(AItemBase* AimedItem);
	void CreateWorldInventory(AItemBase* AimedItem);
	void DeleteWorldInventory();

	UInventorySlot* GetInventorySlot(int32 InventoryIdx, int32 slotIdx);

	void ToggleInventory(bool bOpen);
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData);


	virtual void HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex) override;


protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* EquipInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* WorldInventorySlot;

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

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryRowSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryColSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bEquipInventory;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayerStatus;

	bool bOtherInventory;



	AItem_bag* Bag;

	FDropInventoryItem OnDropRequested;

};
