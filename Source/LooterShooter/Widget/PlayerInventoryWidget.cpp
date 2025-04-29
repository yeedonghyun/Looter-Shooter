#include "PlayerInventoryWidget.h"
#include "../Save/SaveManager.h"
#include "../Inventory/Tooltip.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bWorldInventoryOpen = false;
	bUsingItem = false;

}

void UPlayerInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UPlayerInventoryWidget::AddItemEmptySlot(AItemBase* AimedItem) // 꽉차면 안들어가게
{
	int emptyIdx = FindEmptySlot(PlayerInventoryArray);
	PlayerInventoryArray[emptyIdx]->SetSlotFromItem(AimedItem->ItemData);
}

int UPlayerInventoryWidget::FindEmptySlot(TArray<UInventorySlot*>& SlotArray)
{
	for (int i = 0; i < SlotArray.Num(); i++)
	{
		if (!SlotArray[i]->SlotData.bHaveItem) 
		{ 
			return i;
		}
	}
	return -1;
}


void UPlayerInventoryWidget::CreateWorldInventory(AItemBase* AimedItem)
{
	if (WorldBagInventoryArray.Num() > 0)
	{
		DeleteWorldInventory();
	}

	Bag = Cast<AItem_bag>(AimedItem);

	CreateInventory(WorldBagInventoryArray, WorldInventory->Grid, Bag->Width, Bag->Height, EUnderInventoryType::WORLDBAG);
	bWorldInventoryOpen = true;

	TArray<FSlotData>& Items = Bag->savedItems;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].bHaveItem)
		{
			UInventorySlot* slot = Cast<UInventorySlot>(WorldBagInventoryArray[i]);
			slot->SetSlotFromSlot(Items[i]);
		}
	}

	WorldInventory->SetVisibility(ESlateVisibility::Visible);
	WorldInventory->ItemSlot->SetSlotFromItem(Bag->ItemData);
}

void UPlayerInventoryWidget::DeleteWorldInventory()
{
	//TArray<FSlotData>& Items = Bag->savedItems;

	//for (int32 i = 0; i < WorldBagInventoryArray.Num(); i++)
	//{
	//	Items[i] = WorldBagInventoryArray[i]->SlotData;
	//}

	for (int32 i = 0; i < WorldBagInventoryArray.Num(); i++)
	{
		WorldBagInventoryArray[i]->RemoveFromParent();
	}

	WorldBagInventoryArray.Empty();
	bWorldInventoryOpen = false;
	WorldInventory->SetVisibility(ESlateVisibility::Hidden);
}




void UPlayerInventoryWidget::UseItem(UInventorySlot* TargetSlot)
{
	if (!bUsingItem)
	{
		if (TargetSlot->SlotData.Type == EItemType::HEALING || TargetSlot->SlotData.Type == EItemType::ARMOR)
		{
			TargetSlot->StartConsume();
			OnItemUseRequested.Broadcast(TargetSlot->SlotData);
		}
	}
}



void UPlayerInventoryWidget::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SWAP")));

	if (DraggingSlot->SlotType == EItemType::BAG && TargetSlot->SlotType == EItemType::BAG)
	{
		UItemInventory* FromInventory = DraggingSlot->GetTypedOuter<UItemInventory>();
		UItemInventory* ToInventory = TargetSlot->GetTypedOuter<UItemInventory>();

		UVerticalBox* FromGrid = FromInventory->Grid;
		UVerticalBox* ToGrid = ToInventory->Grid;

		TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
		TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();

		FromGrid->ClearChildren();
		ToGrid->ClearChildren();

		for (UWidget* Child : FromChildren)
		{
			ToGrid->AddChild(Child);
		}
		for (UWidget* Child : ToChildren)
		{
			FromGrid->AddChild(Child);
		}

		SwapSlotData(DraggingSlot, TargetSlot);
		//std::swap(WorldBagInventoryArray, EquipInventoryArray);
	}

	else if (DraggingSlot->SlotType != EItemType::BAG && TargetSlot->SlotType == EItemType::BAG)
	{
		//if (From->SlotData.Type == EItemType::BAG)
		{
			UItemInventory* ToInventory = TargetSlot->GetTypedOuter<UItemInventory>();
			UVerticalBox* ToGrid = ToInventory->Grid;
			TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();
			ToGrid->ClearChildren();
			SwapSlotData(DraggingSlot, TargetSlot);


		}
	}

	else if (DraggingSlot->SlotType == EItemType::BAG && TargetSlot->SlotType != EItemType::BAG)
	{
		if (TargetSlot->SlotData.Type == EItemType::BAG)
		{
			UItemInventory* FromInventory = DraggingSlot->GetTypedOuter<UItemInventory>();
			UVerticalBox* FromGrid = FromInventory->Grid;
			TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
			FromGrid->ClearChildren();

			USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

			if (TSubclassOf<AItem_bag> ItemClass = LoadClass<AItem_bag>(nullptr, *FullPath))
			{
				AItem_bag* DefaultBag = ItemClass->GetDefaultObject<AItem_bag>();
				CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
				SwapSlotData(DraggingSlot, TargetSlot);
			}
		}

	}


	else
	{
		bool bUpdateWorldBagData = false;

		SwapSlotData(DraggingSlot, TargetSlot);

		if (DraggingSlot->UnderInventoryType != TargetSlot->UnderInventoryType)
		{
			TArray<UInventorySlot*>& DragArray = ReturnInventoryArray(DraggingSlot->UnderInventoryType);
			TArray<UInventorySlot*>& TargetArray = ReturnInventoryArray(TargetSlot->UnderInventoryType);

			//std::swap(DragArray, TargetArray);

			if (DraggingSlot->UnderInventoryType == EUnderInventoryType::WORLDBAG)
			{
				bUpdateWorldBagData = true;
			}

			if (TargetSlot->UnderInventoryType == EUnderInventoryType::WORLDBAG)
			{
				bUpdateWorldBagData = true;
			}

			if (bUpdateWorldBagData)
			{
				TArray<FSlotData>& Items = Bag->savedItems;

				for (int32 i = 0; i < WorldBagInventoryArray.Num(); i++)
				{
					Items[i] = WorldBagInventoryArray[i]->SlotData;
				}
			}

		}

	}
}


void UPlayerInventoryWidget::ToggleInventory(bool bOpen)
{
	if (bOpen)
	{
		SetUIMode(ESlateVisibility::Visible, true, FInputModeGameAndUI(), true, 1.0f);
	}

	else
	{
		SetUIMode(ESlateVisibility::Visible, false, FInputModeGameOnly(), false, 0.0f);
		SlotToolTip->SetVisibility(ESlateVisibility::Hidden);
		SlotToolTip->bShouldFollowMouse = false;
	}
}

void UPlayerInventoryWidget::SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData, bool bSetIsEnable, float UIOpacity)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = showCursor;
		PC->SetInputMode(InData);
	}

	SetIsEnabled(bSetIsEnable);
	SetRenderOpacity(UIOpacity);
	SetVisibility(Visible);
}