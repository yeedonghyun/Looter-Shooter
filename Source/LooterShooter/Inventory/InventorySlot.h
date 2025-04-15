// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "LooterShooter/Item/ItemBase.h"

#include "../Item/ItemData.h"

#include "LooterShooter/Item/Item_bag.h"
#include "../Inventory/DragDropSlot.h"
#include "InventorySlot.generated.h"


UENUM(BlueprintType)
enum class ESlotActionType : uint8
{
	DROP UMETA(DisplayName = "DROP"),
	USE UMETA(DisplayName = "USE"),
	CHECK UMETA(DisplayName = "CHECK"),
	DRAG UMETA(DisplayName = "DRAG")
};


DECLARE_EVENT_TwoParams(UInventorySlot, FSwapSlot, UInventorySlot*, UInventorySlot*)

DECLARE_EVENT_ThreeParams(UInventorySlot, FSlotAction, FSlotData, ESlotActionType, bool)


UCLASS()
class LOOTERSHOOTER_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void InitInventorySlot(int idx, int InventoryIdx, EItemType type);


	void SetSlotFromItem(const FItemData& data);
	void SetSlotFromSlot(const FSlotData& data);
	void ToggleSlot();
	void GetItemImage(FString ItemName);

	void DropItem();

	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void RequestSlotAction(FSlotData data, ESlotActionType type, bool bActive);

	void RequestSwap(UInventorySlot* Slot);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
		EItemType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EqiupSlot")
		bool bEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* IMG_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Default;

	UPROPERTY(EditAnywhere, Category = "Slot")
		TSubclassOf<UInventorySlot> DragWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FSlotData SlotData;



	int32 _idx;


	int32 _inventoryIdx; // 체크 필요


	FSlotAction OnSlotActionRequested;

	FSwapSlot OnSwapRequested;


};

