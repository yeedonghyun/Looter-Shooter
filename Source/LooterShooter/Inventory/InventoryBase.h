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

#include "InventoryBase.generated.h"



UCLASS()
class LOOTERSHOOTER_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
	
public:



	void CreateSlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize);

	void SaveSelectSlotData(TArray<UInventorySlot*>& SlotArray, const FString& DataName);
	void LoadSelcectSlotData(TArray<UInventorySlot*>& SlotArray, const FString& DataName);

	int FindEmptySlot(TArray<UInventorySlot*>& SlotArray);
	TArray<FSlotData> ConvertSlotToData(TArray<UInventorySlot*>& SlotArray);

	void DeleteSlotArray(TArray<UInventorySlot*>& SlotArray);
	void SwapSlot(UInventorySlot*& From, UInventorySlot*& To);





	
	virtual void HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex);
	void HandleSlotActionRequest(FSlotData data, ESlotActionType type, bool bActive);
	void CheckToolTip(FSlotData data, bool bActive);
	UTooltip* SlotToolTip;
	bool bDragging;


};
