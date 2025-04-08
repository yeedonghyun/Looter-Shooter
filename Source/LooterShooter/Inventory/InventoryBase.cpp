// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBase.h"
#include "../Save/SaveManager.h"

void UInventoryBase::CreateSlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize)
{
	TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'"));

	if (!InventoryClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Can't find Class")));
		return;
	}

	if (ParentSlot)
	{
		SlotArray.Empty();

		for (int32 i = 0; i < rowSize; i++)
		{
			if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(ParentSlot))
			{
				ParentSlot->AddChild(HorizontalBox);

				for (int32 j = 0; j < colSize; j++)
				{
					if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventoryClass))
					{
						InventorySlot->InitInventorySlot((i * colSize) + j, InventoryIdx, false, i, j);
						InventorySlot->OnSwapRequested.AddUObject(this, &UInventoryBase::HandleSwapRequest);
						InventorySlot->OnSlotActionRequested.AddUObject(this, &UInventoryBase::HandleSlotActionRequest);
						HorizontalBox->AddChild(InventorySlot);
						SlotArray.Add(InventorySlot);
					}
				}
			}
		}
	}
}


void UInventoryBase::SaveSelectSlotData(TArray<UInventorySlot*>& SlotArray, const FString& DataName)
{
	TArray<FSlotData> Items = ConvertSlotToData(SlotArray);
	USaveManager::SaveSelectData(Items, "Save1", DataName);
}

void UInventoryBase::LoadSelcectSlotData(TArray<UInventorySlot*>& SlotArray, const FString& DataName)
{
	TArray<FSlotData> Items = USaveManager::LoadSelectData("Save1", DataName);

	if (Items.Num() != 0)
	{
		for (int32 i = 0; i < SlotArray.Num(); i++)
		{
			if (Items[i].bHaveItem)
			{
				SlotArray[i]->SetSlotFromSlot(Items[i]);
			}
		}
	}
}


int UInventoryBase::FindEmptySlot(TArray<UInventorySlot*>& SlotArray)
{
	for (int i = 0; i < SlotArray.Num(); i++)
	{
		if (!SlotArray[i]->SlotData.bHaveItem) { return i; }
	}
	return -1;
}

TArray<FSlotData> UInventoryBase::ConvertSlotToData(TArray<UInventorySlot*>& SlotArray)
{
	TArray<FSlotData> Items;
	for (int i = 0; i < SlotArray.Num(); i++)
	{
		Items.Add(SlotArray[i]->SlotData);
	}
	return Items;
}

void UInventoryBase::DeleteSlotArray(TArray<UInventorySlot*>& SlotArray)
{
	for (int32 i = 0; i < SlotArray.Num(); i++)
	{
		SlotArray[i]->RemoveFromParent();
	}
	SlotArray.Empty();
}

void UInventoryBase::SwapSlot(UInventorySlot*& From, UInventorySlot*& To)
{
	std::swap(From->SlotData, To->SlotData);

	FSlateBrush FromBrush = From->IMG_Item->Brush;
	FSlateBrush ToBrush = To->IMG_Item->Brush;

	From->IMG_Item->SetBrush(ToBrush);
	To->IMG_Item->SetBrush(FromBrush);

	To->ToggleSlot();
	From->ToggleSlot();
}



void UInventoryBase::HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex)
{
}

void UInventoryBase::HandleSlotActionRequest(FSlotData data, ESlotActionType type, bool bActive)
{
	switch (type)
	{

	case ESlotActionType::DROP:
		break;
	case ESlotActionType::USE:
		break;
	case ESlotActionType::CHECK:

		if (!bDragging)
		{
			CheckToolTip(data, bActive);
		}

		break;
	case ESlotActionType::DRAG:
		bDragging = bActive;
		if (bDragging)
		{
			CheckToolTip(data, !bActive);
		}

		break;
	default:
		break;
	}

}


void UInventoryBase::CheckToolTip(FSlotData data, bool bActive)
{
	if (!bActive)
	{
		SlotToolTip->SetVisibility(ESlateVisibility::Collapsed);
		SlotToolTip->bShouldFollowMouse = false;
	}

	else
	{
		FVector2D MousePosition;
		if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			SlotToolTip->SetPositionInViewport(MousePosition + FVector2D(15.0f, 0.0f));
		}

		FString Info = FString::Printf(
			TEXT("Name : %s\nValue : %d\nWeight : %d\nType : %s"),
			*data.Name,
			data.Value,
			data.Weight,
			//*UEnum::GetValueAsString(data.Type)
			*UEnum::GetDisplayValueAsText(data.Type).ToString()
		);

		SlotToolTip->Information->SetText(FText::FromString(Info));
		SlotToolTip->bShouldFollowMouse = true;
		SlotToolTip->SetVisibility(ESlateVisibility::Visible);

	}
}
