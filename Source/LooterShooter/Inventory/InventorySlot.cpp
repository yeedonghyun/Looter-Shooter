// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventorySlot::InitInventorySlot(int index, int inventoryInedx, bool drag)
{
	AddToViewport();
	idx = index;
	inventoryIdx = inventoryInedx;
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetSlotFromItem(const FItemData& data)
{
	ToggleSlot(ESlateVisibility::Visible, true);
	SlotData.SetSlotFromItemData(data);
	GetItemImage(SlotData.Name);
}

void UInventorySlot::SetSlotFromSlot(const FSlotData& data)
{
	ToggleSlot(ESlateVisibility::Visible, true);
	SlotData = data;
	GetItemImage(SlotData.Name);
}

void UInventorySlot::ToggleSlot(ESlateVisibility Visible, bool isActive)
{
	IMG_Item->SetVisibility(Visible);
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

		RequestDrop(SlotData);


		SlotData.bHaveItem = false;
	}
}


FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (SlotData.bHaveItem) { Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton); }
	}

	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		DropItem();
	}


	return Reply.NativeReply;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDragDropSlot* Operation = NewObject<UDragDropSlot>();
		Operation->PrevSlotIndex = idx;
		Operation->PrevInventoryIdx = inventoryIdx;
		OutOperation = Operation;

		if (DragWidgetClass)
		{
			UInventorySlot* DragWidget = CreateWidget<UInventorySlot>(this, DragWidgetClass);
			DragWidget->SetOwningPlayer(GetOwningPlayer());

			if (DragWidget)
			{
				DragWidget->ToggleSlot(ESlateVisibility::Visible, true);
				UTexture2D* ItemTexture = Cast<UTexture2D>(IMG_Item->Brush.GetResourceObject());
				if (ItemTexture) { DragWidget->IMG_Item->SetBrushFromTexture(ItemTexture); }
				Operation->DefaultDragVisual = DragWidget;

				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Debug::%d"), DragWidget->idx));
			}
		}
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragDropSlot* Operation = Cast<UDragDropSlot>(InOperation);

	if (Operation) { RequestSwap(Operation->PrevInventoryIdx, Operation->PrevSlotIndex);}

	return false;
}


void UInventorySlot::RequestSwap(int32 OtherInventoryIdx, int32 OtherSlotIdx)
{
	OnSwapRequested.Broadcast(OtherInventoryIdx, OtherSlotIdx, this->inventoryIdx, this->idx);
}


void UInventorySlot::RequestDrop(FSlotData data)
{
	OnDropRequested.Broadcast(SlotData);
}
