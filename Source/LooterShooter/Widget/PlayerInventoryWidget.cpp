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

	UpdateMagazine();
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
	if (WorldBagInventoryArray.Num() > 0 || WorldBoxInventoryArray.Num() > 0)
	{
		DeleteWorldInventory();
	}

	bWorldInventoryOpen = true;

	InventoryItem = Cast<AItem_Inventory>(AimedItem);
	TArray<FSlotData>& Items = InventoryItem->savedItems;

	switch (InventoryItem->InventoryType)
	{
	case EInventoryType::BAG:
		CreateInventory(WorldBagInventoryArray, WorldInventory->Grid, InventoryItem->Width, InventoryItem->Height, EUnderInventoryType::WORLDBAG);

		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].bHaveItem)
			{
				UInventorySlot* slot = Cast<UInventorySlot>(WorldBagInventoryArray[i]);
				slot->SetSlotFromSlot(Items[i]);
			}
		}

		WorldInventory->SetVisibility(ESlateVisibility::Visible);
		WorldInventory->ItemSlot->SetSlotFromItem(InventoryItem->ItemData);


		break;

	case EInventoryType::BOX:
		CreateInventory(WorldBoxInventoryArray, WorldBoxInventory->Grid, InventoryItem->Width, InventoryItem->Height, EUnderInventoryType::WORLDBOX);


		for (int i = 0; i < Items.Num(); i++)
		{
			if (Items[i].bHaveItem)
			{
				UInventorySlot* slot = Cast<UInventorySlot>(WorldBoxInventoryArray[i]);
				slot->SetSlotFromSlot(Items[i]);
			}
		}

		WorldBoxInventory->SetVisibility(ESlateVisibility::Visible);

		break;

	default:
		break;
	}





}

void UPlayerInventoryWidget::DeleteWorldInventory()
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Delete")));

	for (int32 i = 0; i < WorldBagInventoryArray.Num(); i++)
	{
		WorldBagInventoryArray[i]->RemoveFromParent();
	}

	WorldBagInventoryArray.Empty();
	WorldInventory->SetVisibility(ESlateVisibility::Collapsed);



	for (int32 i = 0; i < WorldBoxInventoryArray.Num(); i++)
	{
		WorldBoxInventoryArray[i]->RemoveFromParent();
	}

	WorldBoxInventoryArray.Empty();
	WorldBoxInventory->SetVisibility(ESlateVisibility::Collapsed);

	bWorldInventoryOpen = false;
}




void UPlayerInventoryWidget::UseItem(UInventorySlot* TargetSlot)
{
	if (!bUsingItem)
	{
		if (TargetSlot->SlotData.Type == EItemType::HEALING || TargetSlot->SlotData.Type == EItemType::ARMOR || TargetSlot->SlotData.Type == EItemType::AMMO)
		{

			bUsingItem = true;
			TargetSlot->StartConsume();
			OnItemUseRequested.Broadcast(TargetSlot->SlotData);

			SaveInventories();
		}
	}
}


void UPlayerInventoryWidget::DropItem(UInventorySlot* TargetSlot)
{
	if (!bUsingItem)
	{
		bUsingItem = true;

		OnDropRequested.Broadcast(TargetSlot->SlotData.Name);

		TargetSlot->SlotData.bHaveItem = false;
		TargetSlot->ToggleSlot();

		SaveInventories();
	}
}



void UPlayerInventoryWidget::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SWAP")));

	if (DraggingSlot->SlotType == EItemType::INVENTORY && TargetSlot->SlotType == EItemType::INVENTORY)
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

	else if (DraggingSlot->SlotType != EItemType::INVENTORY && TargetSlot->SlotType == EItemType::INVENTORY)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("TargetSlotINVENTORY")));


		bool bHaveInventoryItem = false;

		UItemInventory* inven = TargetSlot->GetTypedOuter<UItemInventory>();

		if (inven->InventoryName == "Equip")
		{
			for (int32 i = 0; i < EquipInventoryArray.Num(); i++)
			{
				if (EquipInventoryArray[i]->SlotData.bHaveItem)
				{
					bHaveInventoryItem = true;
					break;
				}
			}
		}

		if (bHaveInventoryItem)
		{
			if (InventoryWarningMessage)
			{
				ShowWarningMessage("Have Item in Bag");
			}

		}

		else
		{
			UItemInventory* ToInventory = TargetSlot->GetTypedOuter<UItemInventory>();
			UVerticalBox* ToGrid = ToInventory->Grid;
			TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();
			ToGrid->ClearChildren();
			SwapSlotData(DraggingSlot, TargetSlot);

			if (ToInventory->ItemSlot->UnderInventoryType == EUnderInventoryType::WORLDBAG)
			{
				DeleteWorldInventory();
			}

			bHaveEquipInventory = false;
		}

	}

	else if (DraggingSlot->SlotType == EItemType::INVENTORY && TargetSlot->SlotType != EItemType::INVENTORY)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("DraggingSlotINVENTORY")));


		if (TargetSlot->SlotData.Type == EItemType::INVENTORY)
		{
			UItemInventory* FromInventory = DraggingSlot->GetTypedOuter<UItemInventory>();
			UVerticalBox* FromGrid = FromInventory->Grid;
			TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
			FromGrid->ClearChildren();

			SwapSlotData(DraggingSlot, TargetSlot);

			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *DraggingSlot->SlotData.Name, *DraggingSlot->SlotData.Name);

			if (TSubclassOf<AItem_Inventory> ItemClass = LoadClass<AItem_Inventory>(nullptr, *FullPath))
			{
				AItem_Inventory* DefaultBag = ItemClass->GetDefaultObject<AItem_Inventory>();
				CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
				//EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
				bHaveEquipInventory = true;
			}
		}

	}

	else if (DraggingSlot->SlotData.Type == EItemType::AMMO && TargetSlot->SlotData.Type == EItemType::AMMO)
	{
		int NeedAmmo = DraggingSlot->SlotData.MaxAmount - DraggingSlot->SlotData.Amount;
		int maxBringable = FMath::Min(NeedAmmo, TargetSlot->SlotData.Amount);

		DraggingSlot->SlotData.Amount += maxBringable;
		TargetSlot->SlotData.Amount -= maxBringable;

		DraggingSlot->ToggleSlot();

		if (TargetSlot->SlotData.Amount == 0)
		{
			TargetSlot->SlotData.bHaveItem = false;
		}

		TargetSlot->ToggleSlot();

	}



	else
	{
		bool bUpdateWorldBagData = false;
		bool bUpdateWorldBoxData = false;

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("SWAPDATA")));

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

			if (DraggingSlot->UnderInventoryType == EUnderInventoryType::WORLDBOX)
			{
				bUpdateWorldBoxData = true;
			}

			if (TargetSlot->UnderInventoryType == EUnderInventoryType::WORLDBAG)
			{
				bUpdateWorldBagData = true;
			}

			if (TargetSlot->UnderInventoryType == EUnderInventoryType::WORLDBOX)
			{
				bUpdateWorldBoxData = true;
			}

			if (bUpdateWorldBagData)
			{
				TArray<FSlotData>& Items = InventoryItem->savedItems;

				for (int32 i = 0; i < WorldBagInventoryArray.Num(); i++)
				{
					Items[i] = WorldBagInventoryArray[i]->SlotData;
				}
			}

			if (bUpdateWorldBoxData)
			{
				TArray<FSlotData>& Items = InventoryItem->savedItems;

				for (int32 i = 0; i < WorldBoxInventoryArray.Num(); i++)
				{
					Items[i] = WorldBoxInventoryArray[i]->SlotData;
				}
			}

		}

	}

	UpdateMagazine();
	SaveInventories();
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

void UPlayerInventoryWidget::UpdateMagazine()
{
	CheckAmmo();

	OnUpdateMagazineRequested.Broadcast(sumAmmo);
}


int UPlayerInventoryWidget::GetAmmo(int needAmmo)
{
	int ammoTaken = 0;
	int ammoToTakeFromSlot = FMath::Min(needAmmo, FirstAmmoSlot->SlotData.Amount);

	ammoTaken += ammoToTakeFromSlot;
	needAmmo -= ammoToTakeFromSlot;
	FirstAmmoSlot->SlotData.Amount -= ammoToTakeFromSlot;
	sumAmmo -= ammoToTakeFromSlot;

	FString fs = FString::FromInt(FirstAmmoSlot->SlotData.Amount);
	FirstAmmoSlot->Amount->SetText(FText::FromString(fs));

	if (FirstAmmoSlot->SlotData.Amount == 0)
	{
		FirstAmmoSlot->SlotData.bHaveItem = false;
		FirstAmmoSlot->ToggleSlot();
		CheckAmmo();
	}

	if (needAmmo > 0)
	{
		ammoToTakeFromSlot = FMath::Min(needAmmo, FirstAmmoSlot->SlotData.Amount);

		ammoTaken += ammoToTakeFromSlot;
		needAmmo -= ammoToTakeFromSlot;
		FirstAmmoSlot->SlotData.Amount -= ammoToTakeFromSlot;

		fs = FString::FromInt(FirstAmmoSlot->SlotData.Amount);
		FirstAmmoSlot->Amount->SetText(FText::FromString(fs));
		sumAmmo -= ammoToTakeFromSlot;

		if (FirstAmmoSlot->SlotData.Amount == 0)
		{
			FirstAmmoSlot->SlotData.bHaveItem = false;
			FirstAmmoSlot->ToggleSlot();
			CheckAmmo();
		}
	}

	return ammoTaken;
}
