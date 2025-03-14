// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "../Inventory/DragDropSlot.h"
#include "LooterShooter/Item/ItemBase.h"

#include "InventorySlot.generated.h"

DECLARE_EVENT_TwoParams(UInventorySlot, FChangeSlot, int32, int32)


UCLASS()
class LOOTERSHOOTER_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetVisible();
	void SetInvisible();
	void SetColor(float r, float g, float b, float a);
	void SetTexture();
	void UpdateSlot();
	void RequestSwap(int32 OtherSlotIdx);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Item;
		 
	UPROPERTY(meta = (BindWidget))
	UImage* Default;

	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<UInventorySlot> DragWidgetClass;

	bool haveItem;
	int32 idx;

	EItemType itemType;

	FChangeSlot OnSwapRequested;

};

