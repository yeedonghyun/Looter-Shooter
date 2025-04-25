// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"



void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();


	ProgressBar->SetVisibility(ESlateVisibility::Hidden);

	bUseItem = false;
	ItemUseDuration = 0.f;
	ItemUseDelay = 1.f;
}

void UInventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bUseItem)
	{
		ItemUseDuration += InDeltaTime;

		if (ItemUseDuration >= ItemUseDelay) { CompleteConsume(); }
		else { ProgressBar->SetPercentage(ItemUseDuration); }
	}
}


void UInventorySlot::CreateInventorySlot(int idx, EItemType type)
{
	AddToViewport();

	_idx = idx;
	SlotType = type;
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
}






void UInventorySlot::InitInventorySlot(int idx, int InventoryIdx, EItemType type)
{
	AddToViewport();
	_idx = idx;
	_inventoryIdx = InventoryIdx;
	SlotType = type;
	bEquipped = false;
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::StartConsume()
{
	bUseItem = true;
	ProgressBar->SetVisibility(ESlateVisibility::Visible);
	//ItemUseDelay
}

void UInventorySlot::CompleteConsume()
{
	SlotData.bHaveItem = false;
	bUseItem = false;
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);

	ItemUseDuration = 0.f;
	ProgressBar->SetPercentage(ItemUseDuration);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
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
		RequestSlotAction(ESlotActionType::CHECK, false);
	}

	else
	{
		RequestSlotAction(ESlotActionType::CHECK, true);
	}

}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RequestSlotAction(ESlotActionType::CHECK, false);
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
			RequestSlotAction(ESlotActionType::DRAG, true);
		}
	}

	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		RequestSlotAction(ESlotActionType::USE, true);
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

		Operation->DraggingSlot = this;


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


	if (Operation) { RequestSwap(Operation->DraggingSlot); }

	RequestSlotAction(ESlotActionType::DRAG, false);

	return false;
}


void UInventorySlot::RequestSlotAction(ESlotActionType type, bool bActive)
{
	OnSlotActionRequested.Broadcast(this, type, bActive);
}


void UInventorySlot::RequestSwap(UInventorySlot* TargetSlot)
{
	OnSwapRequested.Broadcast(this, TargetSlot);
}

