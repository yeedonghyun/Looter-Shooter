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

#include "InventoryBase.generated.h"




UCLASS()
class LOOTERSHOOTER_API UInventoryBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void CreateSlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize);
	void SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData);


	TArray<FSlotData> ConvertSlotToData(TArray<UInventorySlot*>& SlotArray);

	void DeleteSlotArray(TArray<UInventorySlot*>& SlotArray);
	void SwapSlot(UInventorySlot*& From, UInventorySlot*& To);

	virtual void HandleSwapRequest(UInventorySlot* From, UInventorySlot* To);


	virtual void UseItem(FItemData data);




	
	void HandleSlotActionRequest(FSlotData data, ESlotActionType type, bool bActive);
	void CheckToolTip(FSlotData data, bool bActive);


	void ApplyStatByType(EItemType Type, int32 Value);


	UTooltip* SlotToolTip;
	bool bDragging;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlayerStatus;



	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

};
