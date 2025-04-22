#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
	LoadInventoryData();

	if (ReturnMain)
	{
		ReturnMain->OnClicked.AddDynamic(this, &UStorageUserWidget::OnReturnMainButtonClicked);
	}

	if (this->SaveButton)
	{
		this->SaveButton->OnClicked.AddDynamic(this, &UStorageUserWidget::OnSaveButtonClicked);
	}
}

void UStorageUserWidget::InitWidget()
{
	//bOtherInventory = false;
	bDragging = false;

	EquipInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UStorageUserWidget::HandleSwapRequest);
	EquipInventory->ItemSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

	ArmorSlot->OnSwapRequested.AddUObject(this, &UStorageUserWidget::HandleSwapRequest);
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
void UStorageUserWidget::LoadInventoryData()
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

			//EquipInventory->Grid

			CreateSlots(EquipInventory->Grid, EquipInventoryArray, 2, DefaultBag->Width, DefaultBag->Height);
			EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
		}

		if (SaveData->EquipInventoryItems.Num() != 0)
		{
			SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
		}
	}


	CreateSlots(StorageInventory->Grid, StorageInventoryArray, 0, SaveData->StorageRowSize, SaveData->StorageColSize);

	if (SaveData->StorageItems.Num() != 0)
	{
		SetArrayData(StorageInventoryArray, SaveData->StorageItems);
	}

}

void UStorageUserWidget::OnReturnMainButtonClicked()
{
	this->RemoveFromParent();

	if (TSubclassOf<UUserWidget> SelectMapWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_SelectMapUserWidget.BP_SelectMapUserWidget_C'")))
	{
		if (UUserWidget* SelectMapWidget = CreateWidget<UUserWidget>(GetWorld(), SelectMapWidgetClass))
		{
			SelectMapWidget->AddToViewport();
		}
	}




}


void UStorageUserWidget::OnSaveButtonClicked()
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

	TArray<FSlotData> tmp3;
	for (int i = 0; i < StorageInventoryArray.Num(); i++)
	{
		tmp3.Add(StorageInventoryArray[i]->SlotData);
	}

	SaveData->StorageItems = tmp3;


	USaveManager::SaveDataSet("Save1", SaveData);

}

void UStorageUserWidget::UseItem(FItemData data)
{

}


void UStorageUserWidget::HandleSwapRequest(UInventorySlot* From, UInventorySlot* To)
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
		//std::swap(WorldInventoryArray, EquipInventoryArray);

	}

	else if (From->SlotType != EItemType::BAG && To->SlotType == EItemType::BAG)
	{

		UItemInventory* ToInventory = To->GetTypedOuter<UItemInventory>();
		UVerticalBox* ToGrid = ToInventory->Grid;
		TArray<UWidget*> ToChildren = ToGrid->GetAllChildren();
		ToGrid->ClearChildren();
		SwapSlot(From, To);
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