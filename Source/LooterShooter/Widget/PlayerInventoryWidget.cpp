#include "PlayerInventoryWidget.h"
#include "../Save/SaveManager.h"
#include "../Inventory/Tooltip.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
	LoadInventoryData();

	if (this->SaveButton)
	{
		this->SaveButton->OnClicked.AddDynamic(this, &UPlayerInventoryWidget::OnSaveButtonClicked);
	}
}

void UPlayerInventoryWidget::InitWidget()
{
	bOtherInventory = false;
	bDragging = false;

	WorldInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	WorldInventory->SetVisibility(ESlateVisibility::Hidden);

	EquipInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	EquipInventory->ItemSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

	ArmorSlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	ArmorSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

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

void UPlayerInventoryWidget::LoadInventoryData()
{
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	CreateSlots(PlayerInventory->Grid, PlayerInventoryArray, 0, SaveData->InventoryRowSize, SaveData->InventoryColSize);

	if (SaveData->InventoryItems.Num() != 0)
	{
		SetArrayData(PlayerInventoryArray, SaveData->InventoryItems);
	}

	if (SaveData->bEquipInventory)
	{
		FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

		if (TSubclassOf<AItem_bag> ItemClass = LoadClass<AItem_bag>(nullptr, *FullPath))
		{
			AItem_bag* DefaultBag = ItemClass->GetDefaultObject<AItem_bag>();

			CreateSlots(EquipInventory->Grid, EquipInventoryArray, 2, DefaultBag->Width, DefaultBag->Height);
			EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
		}

		if (SaveData->EquipInventoryItems.Num() != 0)
		{
			SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
		}
	}
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
		if (!SlotArray[i]->SlotData.bHaveItem) { return i; }
	}

	return -1;
}


void UPlayerInventoryWidget::CreateWorldInventory(AItemBase* AimedItem) // 드래그앤 드랍시를 생각
{
	if (WorldInventoryArray.Num() > 0)
	{
		DeleteWorldInventory();
	}

	Bag = Cast<AItem_bag>(AimedItem);
	//AItem_bag* Bag = Cast<AItem_bag>(AimedItem);
	CreateSlots(WorldInventory->Grid, WorldInventoryArray, 1, Bag->Width, Bag->Height);
	bOtherInventory = true;

	TArray<FSlotData>& Items = Bag->savedItems;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].bHaveItem)
		{
			UInventorySlot* slot = Cast<UInventorySlot>(WorldInventoryArray[i]);
			slot->SetSlotFromSlot(Items[i]);
		}
	}

	WorldInventory->SetVisibility(ESlateVisibility::Visible);
	WorldInventory->ItemSlot->SetSlotFromItem(Bag->ItemData);
}

void UPlayerInventoryWidget::DeleteWorldInventory()
{
	TArray<FSlotData>& Items = Bag->savedItems;

	for (int32 i = 0; i < WorldInventoryArray.Num(); i++)
	{
		Items[i] = WorldInventoryArray[i]->SlotData;
	}

	for (int32 i = 0; i < WorldInventoryArray.Num(); i++)
	{
		WorldInventoryArray[i]->RemoveFromParent();
	}

	WorldInventoryArray.Empty();
	bOtherInventory = false;
	WorldInventory->SetVisibility(ESlateVisibility::Hidden);
}



void UPlayerInventoryWidget::ToggleInventory(bool bOpen)
{
	if (bOpen)
	{ 
		SetUIMode(ESlateVisibility::Visible, true, FInputModeGameAndUI()); 
	}

	else 
	{ 
		SetUIMode(ESlateVisibility::Hidden, false, FInputModeGameOnly()); 

		SlotToolTip->SetVisibility(ESlateVisibility::Hidden);
		SlotToolTip->bShouldFollowMouse = false;
	}
}

void UPlayerInventoryWidget::SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = showCursor;
		PC->SetInputMode(InData);
	}

	SetVisibility(Visible);
}


void UPlayerInventoryWidget::UseItem(FItemData data)
{
	OnItemUseRequested.Broadcast(data);
}


void UPlayerInventoryWidget::OnSaveButtonClicked()
{
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	TArray<FSlotData> tmp;
	for (int i = 0; i < PlayerInventoryArray.Num(); i++)
	{
		tmp.Add(PlayerInventoryArray[i]->SlotData);
	}

	SaveData->InventoryItems = tmp;

	TArray<FSlotData> tmp2;
	for (int i = 0; i < EquipInventoryArray.Num(); i++)
	{
		tmp2.Add(EquipInventoryArray[i]->SlotData);
	}

	SaveData->EquipInventoryItems = tmp2;


	USaveManager::SaveDataSet("Save1", SaveData);

}

void UPlayerInventoryWidget::HandleSwapRequest(UInventorySlot* From, UInventorySlot* To)
{
	if (From->SlotType == EItemType::BAG && To->SlotType == EItemType::BAG)
	{
		UItemInventory* FromInventory = From->GetTypedOuter<UItemInventory>();
		UItemInventory* ToInventory = To->GetTypedOuter<UItemInventory>();

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

		SwapSlot(From, To);
		std::swap(WorldInventoryArray, EquipInventoryArray);

	}

	else if (From->SlotType != EItemType::BAG && To->SlotType == EItemType::BAG)
	{
		//if (From->SlotData.Type == EItemType::BAG)
		{
			UItemInventory* ToInventory = To->GetTypedOuter<UItemInventory>();
			UVerticalBox* ToGrid = ToInventory->Grid;
			TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();
			ToGrid->ClearChildren();
			SwapSlot(From, To);


		}
	}

	else if (From->SlotType == EItemType::BAG && To->SlotType != EItemType::BAG)
	{
		if (To->SlotData.Type == EItemType::BAG)
		{
			UItemInventory* FromInventory = From->GetTypedOuter<UItemInventory>();
			UVerticalBox* FromGrid = FromInventory->Grid;
			TArray<UWidget*> FromChildren = FromGrid->GetAllChildren();
			FromGrid->ClearChildren();

			USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

			if (TSubclassOf<AItem_bag> ItemClass = LoadClass<AItem_bag>(nullptr, *FullPath))
			{
				AItem_bag* DefaultBag = ItemClass->GetDefaultObject<AItem_bag>();

				CreateSlots(FromGrid, EquipInventoryArray, 2, DefaultBag->Width, DefaultBag->Height);
				//FromInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
				SwapSlot(From, To);
			}
		}


	}


	else
	{
		SwapSlot(From, To);
	}



}