// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	bDragging = false;

	if (TSubclassOf<UUserWidget> ToolTip = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Inventory/BP_Tooltip.BP_Tooltip_C'")))
	{
		SlotToolTip = CreateWidget<UTooltip>(GetWorld(), ToolTip);

		if (SlotToolTip)
		{
			SlotToolTip->AddToViewport(999);
			SlotToolTip->SetVisibility(ESlateVisibility::Hidden);
		}
	}


}


void UInventoryWidgetBase::CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize)
{
	if (TSubclassOf<UUserWidget> InventorySlotClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'")))
	{
		if (ParentGrid)
		{
			SlotArray.Empty();

			for (int32 i = 0; i < rowSize; i++)
			{
				if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(ParentGrid))
				{
					ParentGrid->AddChild(HorizontalBox);

					for (int32 j = 0; j < colSize; j++)
					{
						if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass))
						{
							InventorySlot->CreateInventorySlot((i * colSize) + j, EItemType::STUFF);
							InventorySlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
							InventorySlot->OnSlotActionRequested.AddUObject(this, &UInventoryWidgetBase::HandleSlotActionRequest);
							HorizontalBox->AddChild(InventorySlot);

							SlotArray.Add(InventorySlot);
						}
					}
				}
			}
		}
	}
}

void UInventoryWidgetBase::CreateItemInventory(TArray<UInventorySlot*>& SlotArray, UInventorySlot* ItemSlot, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize)
{
	//ItemSlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
	CreateInventory(SlotArray, ParentGrid, rowSize, colSize);
}



void UInventoryWidgetBase::SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData)
{
	for (int32 i = 0; i < SlotArray.Num(); i++)
	{
		if (ArrayData[i].bHaveItem)
		{
			SlotArray[i]->SetSlotFromSlot(ArrayData[i]);
		}
	}
}

void UInventoryWidgetBase::SwapSlotData(UInventorySlot*& DraggingSlot, UInventorySlot*& TargetSlot)
{
	std::swap(DraggingSlot->SlotData, TargetSlot->SlotData);

	FSlateBrush FromBrush = DraggingSlot->IMG_Item->Brush;
	FSlateBrush ToBrush = TargetSlot->IMG_Item->Brush;

	DraggingSlot->IMG_Item->SetBrush(ToBrush);
	TargetSlot->IMG_Item->SetBrush(FromBrush);

	TargetSlot->ToggleSlot();
	DraggingSlot->ToggleSlot();
}



void UInventoryWidgetBase::HandleSlotActionRequest(UInventorySlot* TargetSlot, ESlotActionType type, bool bActive)
{
	switch (type)
	{
	case ESlotActionType::DROP:
		break;

	case ESlotActionType::USE:
		UseItem(TargetSlot);
		break;

	case ESlotActionType::CHECK:
		if (!bDragging && SlotToolTip)
		{
			SlotToolTip->UpdateTexts(TargetSlot->SlotData, bActive);
		}
		break;

	case ESlotActionType::DRAG:
		bDragging = bActive;
		if (bDragging && SlotToolTip)
		{
			SlotToolTip->UpdateTexts(TargetSlot->SlotData, !bActive);
		}

		break;

	default:
		break;
	}

}


void UInventoryWidgetBase::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{

}

void UInventoryWidgetBase::UseItem(UInventorySlot* TargetSlot)
{
}


