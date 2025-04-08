// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventorySlot::InitIdxes(int idx, int inventoryIdx)
{
	_idx = idx;
	_inventoryIdx = inventoryIdx;
}


void UInventorySlot::InitInventorySlot(int index, int inventoryInedx, bool drag, int32 x, int32 y)
{
	AddToViewport();
	_idx = index;
	_inventoryIdx = inventoryInedx;
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetSlotFromItem(const FItemData& data)
{
	SlotData.SetSlotFromItemData(data);
	GetItemImage(SlotData.Name);
	ToggleSlot();
}

void UInventorySlot::SetSlotFromSlot(const FSlotData& data)
{
	SlotData = data;
	GetItemImage(SlotData.Name);
	ToggleSlot();
}

void UInventorySlot::ToggleSlot()
{
	if (SlotData.bHaveItem)
	{
		IMG_Item->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		IMG_Item->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::GetItemImage(FString ItemName)
{
	FString AssetPath = FString::Format(TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Inventory/items/{0}.{0}'"), { ItemName });
	UTexture2D* NewAimImage = LoadObject<UTexture2D>(nullptr, *AssetPath);
	IMG_Item->SetBrushFromTexture(NewAimImage);
}


void UInventorySlot::DropItem()
{
	if (SlotData.bHaveItem)
	{
		IMG_Item->SetVisibility(ESlateVisibility::Hidden);

		//RequestDrop(SlotData);

		SlotData.bHaveItem = false;
	}
}


void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (!SlotData.bHaveItem)
	{
		RequestSlotAction(SlotData, ESlotActionType::CHECK, false);
	}

	else
	{
		RequestSlotAction(SlotData, ESlotActionType::CHECK, true);
	}

}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RequestSlotAction(SlotData, ESlotActionType::CHECK, false);
}




FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (SlotData.bHaveItem) 
		{ 
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton); 
			RequestSlotAction(SlotData, ESlotActionType::DRAG, true);
		}
	}

	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		//RequestUse(SlotData);
	}


	return Reply.NativeReply;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDragDropSlot* Operation = NewObject<UDragDropSlot>();
		Operation->PrevSlotIndex = _idx;
		Operation->PrevInventoryIdx = _inventoryIdx;
		OutOperation = Operation;

		if (DragWidgetClass)
		{
			UInventorySlot* DragWidget = CreateWidget<UInventorySlot>(this, DragWidgetClass);
			DragWidget->SetOwningPlayer(GetOwningPlayer());

			if (DragWidget)
			{
				DragWidget->SlotData.bHaveItem = true;
				DragWidget->ToggleSlot();
				UTexture2D* ItemTexture = Cast<UTexture2D>(IMG_Item->Brush.GetResourceObject());
				if (ItemTexture) { DragWidget->IMG_Item->SetBrushFromTexture(ItemTexture); }
				Operation->DefaultDragVisual = DragWidget;

				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Debug::%d"), DragWidget->_idx));
			}
		}
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragDropSlot* Operation = Cast<UDragDropSlot>(InOperation);

	if (Operation) { RequestSwap(Operation->PrevInventoryIdx, Operation->PrevSlotIndex);}

	RequestSlotAction(SlotData, ESlotActionType::DRAG, false);

	return false;
}


void UInventorySlot::RequestSwap(int32 OtherInventoryIdx, int32 OtherSlotIdx)
{
	OnSwapRequested.Broadcast(OtherInventoryIdx, OtherSlotIdx, this->_inventoryIdx, this->_idx);
}

void UInventorySlot::RequestSlotAction(FSlotData data, ESlotActionType type, bool bActive)
{
	OnSlotActionRequested.Broadcast(SlotData, type, bActive);
}
