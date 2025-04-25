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
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"


#include "InventoryWidgetBase.generated.h"


UENUM(BlueprintType)
enum class EInventoryWidgetType : uint8
{
	PLAY UMETA(DisplayName = "PLAY"),
	STORAGE UMETA(DisplayName = "STORAGE")
};


UCLASS()
class LOOTERSHOOTER_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryWidgetBase")
	EInventoryWidgetType InventoryWidgetType;

	bool bDragging;


	void CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize);
	void CreateItemInventory(TArray<UInventorySlot*>& SlotArray, UInventorySlot* ItemSlot, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize);
	void SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData);
	void SwapSlotData(UInventorySlot*& DraggingSlot, UInventorySlot*& TargetSlot);

	void HandleSlotActionRequest(UInventorySlot* TargetSlot, ESlotActionType type, bool bActive);
	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot);
	virtual void UseItem(UInventorySlot* TargetSlot);



	UTooltip* SlotToolTip;

	UPROPERTY(meta = (BindWidgetOptional))
		UButton* SaveButton;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	//UItemInventory* WorldBoxInventory;

};
