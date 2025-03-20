// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "../Inventory/DragDropSlot.h"
#include "LooterShooter/Item/ItemBase.h"
#include "../Inventory/InventoryItem.h"
#include "LooterShooter/Item/Item_bag.h"
#include "InventorySlot.generated.h"


DECLARE_EVENT_FourParams(UInventorySlot, FChangeSlot, int32, int32, int32, int32)


UCLASS()
class LOOTERSHOOTER_API UInventorySlot : public UInventoryItem
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void InitInventorySlot(int index, int inventoryInedx, bool drag);
	void SetItem(AItemBase* AimedItem);
	void ToggleSlot(ESlateVisibility Visible, bool isActive);
	void GetWorldItemData(AItemBase* AimedItem);
	void GetItemImage(FString ItemName);
	void GetBagData(FSavedItem bag);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	void RequestSwap(int32 OtherInventoryIdx, int32 OtherSlotIdx);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* IMG_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Default;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UInventorySlot> DragWidgetClass;

	bool bHaveItem;
	int32 idx;
	int32 inventoryIdx;
	FChangeSlot OnSwapRequested;
};

