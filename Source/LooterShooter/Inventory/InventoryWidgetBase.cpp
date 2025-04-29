// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	bDragging = false;
	bHaveEquipInventory = false;

	InitWidget();

	if (TSubclassOf<UUserWidget> ToolTip = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Inventory/BP_Tooltip.BP_Tooltip_C'")))
	{
		SlotToolTip = CreateWidget<UTooltip>(GetWorld(), ToolTip);

		if (SlotToolTip)
		{
			SlotToolTip->AddToViewport(999);
			SlotToolTip->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (this->SaveButton)
	{
		this->SaveButton->OnClicked.AddDynamic(this, &UInventoryWidgetBase::SaveInventories);
	}
}


void UInventoryWidgetBase::InitWidget()
{
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	if (PlayerInventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerInventory")));

		CreateInventory(PlayerInventoryArray, PlayerInventory->Grid, SaveData->InventoryRowSize, SaveData->InventoryColSize, EUnderInventoryType::PLAYER);

		if (SaveData->InventoryItems.Num() != 0)
		{
			SetArrayData(PlayerInventoryArray, SaveData->InventoryItems);
		}
	}

	if (EquipInventory)
	{
		if (SaveData->bEquipInventory)
		{
			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

			if (TSubclassOf<AItem_bag> ItemClass = LoadClass<AItem_bag>(nullptr, *FullPath))
			{
				AItem_bag* DefaultBag = ItemClass->GetDefaultObject<AItem_bag>();
				CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
				EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
			}

			if (SaveData->EquipInventoryItems.Num() != 0)
			{
				SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
			}
		}
	}

	if (StorageInventory)
	{
		CreateInventory(StorageInventoryArray, StorageInventory->Grid, SaveData->StorageRowSize, SaveData->StorageColSize, EUnderInventoryType::STORAGE);

		if (SaveData->StorageItems.Num() != 0)
		{
			SetArrayData(StorageInventoryArray, SaveData->StorageItems);
		}
	}


	if (EquipInventory)
	{
		EquipInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
		EquipInventory->ItemSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);
	}

	if (WorldInventory)
	{
		WorldInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
		WorldInventory->SetVisibility(ESlateVisibility::Hidden);
	}

}



void UInventoryWidgetBase::CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize, EUnderInventoryType InventoryType)
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
							InventorySlot->CreateInventorySlot((i * colSize) + j, EItemType::STUFF, InventoryType);
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
	//if (DraggingSlot->SlotType == EItemType::BAG && TargetSlot->SlotType == EItemType::BAG)
	//{
	//	UItemInventory* FromInventory = DraggingSlot->GetTypedOuter<UItemInventory>();
	//	UItemInventory* ToInventory = TargetSlot->GetTypedOuter<UItemInventory>();

	//	UVerticalBox* FromGrid = FromInventory->Grid;
	//	UVerticalBox* ToGrid = ToInventory->Grid;

	//	TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
	//	TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();

	//	FromGrid->ClearChildren();
	//	ToGrid->ClearChildren();

	//	for (UWidget* Child : FromChildren)
	//	{
	//		ToGrid->AddChild(Child);
	//	}
	//	for (UWidget* Child : ToChildren)
	//	{
	//		FromGrid->AddChild(Child);
	//	}

	//	SwapSlotData(DraggingSlot, TargetSlot);
	//	std::swap(WorldBagInventoryArray, EquipInventoryArray);

	//}

	//else if (DraggingSlot->SlotType != EItemType::BAG && TargetSlot->SlotType == EItemType::BAG)
	//{
	//	UItemInventory* ToInventory = TargetSlot->GetTypedOuter<UItemInventory>();
	//	UVerticalBox* ToGrid = ToInventory->Grid;
	//	TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();
	//	ToGrid->ClearChildren();
	//	SwapSlotData(DraggingSlot, TargetSlot);
	//}

	//else if (DraggingSlot->SlotType == EItemType::BAG && TargetSlot->SlotType != EItemType::BAG)
	//{
	//	if (TargetSlot->SlotData.Type == EItemType::BAG)
	//	{
	//		UItemInventory* FromInventory = DraggingSlot->GetTypedOuter<UItemInventory>();
	//		UVerticalBox* FromGrid = FromInventory->Grid;
	//		TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
	//		FromGrid->ClearChildren();

	//		USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	//		FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

	//		if (TSubclassOf<AItem_bag> ItemClass = LoadClass<AItem_bag>(nullptr, *FullPath))
	//		{
	//			AItem_bag* DefaultBag = ItemClass->GetDefaultObject<AItem_bag>();
	//			CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
	//			SwapSlotData(DraggingSlot, TargetSlot);
	//		}
	//	}

	//}


	//else
	//{
	//	SwapSlotData(DraggingSlot, TargetSlot);
	//}
}

void UInventoryWidgetBase::UseItem(UInventorySlot* TargetSlot)
{
}



void UInventoryWidgetBase::SaveInventories()
{
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	if (PlayerInventory)
	{
		TArray<FSlotData> tmp;
		for (int i = 0; i < PlayerInventoryArray.Num(); i++)
		{
			tmp.Add(PlayerInventoryArray[i]->SlotData);
		}
		SaveData->InventoryItems = tmp;
	}

	if (EquipInventory)
	{
		TArray<FSlotData> tmp;
		for (int i = 0; i < EquipInventoryArray.Num(); i++)
		{
			tmp.Add(EquipInventoryArray[i]->SlotData);
		}

		SaveData->EquipInventoryItems = tmp;
	}

	if (StorageInventory)
	{
		TArray<FSlotData> tmp;
		for (int i = 0; i < StorageInventoryArray.Num(); i++)
		{
			tmp.Add(StorageInventoryArray[i]->SlotData);
		}

		SaveData->StorageItems = tmp;
	}
	
	USaveManager::SaveDataSet("Save1", SaveData);
}

TArray<UInventorySlot*>& UInventoryWidgetBase::ReturnInventoryArray(EUnderInventoryType InventoryType)
{
	switch (InventoryType)
	{
	case EUnderInventoryType::PLAYER: return PlayerInventoryArray;
	case EUnderInventoryType::EQUIP: return EquipInventoryArray;
	case EUnderInventoryType::WORLDBAG: return WorldBagInventoryArray;
	case EUnderInventoryType::WORLDBOX: return WorldBoxInventoryArray;
	case EUnderInventoryType::STORAGE: return StorageInventoryArray;

	}


	return PlayerInventoryArray;
}

