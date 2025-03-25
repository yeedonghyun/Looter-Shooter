// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Item/Item_bag.h"


#include "PlayerInventoryWidget.generated.h"

DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FDropInventoryItem, FString)

UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void InitPlayerInventorySlots();
	void InitInventorySlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize);
	void ToggleInventory();
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData);
	void AddInventoryItem(AItemBase* AimedItem);
	void CreateOtherInventory(AItemBase* AimedItem);
	void DeleteOtherInventory();
	void HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex);
	void HandleDropRequest(FSlotData data);
	UInventorySlot* GetInventorySlot(int32 InventoryIdx, int32 slotIdx);

	void ChangeOtherInventoryData(FSlotData& Item, const UInventorySlot& slot);
	void SetOtherInventoryImage(FString ItemName);

protected:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerInventorySlots;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OtherInventorySlots;

	TArray<UInventorySlot*> PlayerInventorySlotArray;
	TArray<UInventorySlot*> OtherInventorySlotArray;

public:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryRowSize;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 InventoryColSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IMG_OtherInventory;

	bool bPlayerInventory;
	bool bOtherInventory;

	AItem_bag* Bag;

	FDropInventoryItem OnDropRequested;

};
