#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
	//LoadInventoryData();


	EquipInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UStorageUserWidget::HandleSwapRequest);
	EquipInventory->ItemSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

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
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	CreateInventory(PlayerInventoryArray, PlayerInventory->Grid, SaveData->InventoryRowSize, SaveData->InventoryColSize);

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

			CreateItemInventory(EquipInventoryArray, EquipInventory->ItemSlot, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height);
			EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
		}

		if (SaveData->EquipInventoryItems.Num() != 0)
		{
			SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
		}
	}

	CreateInventory(StorageInventoryArray, StorageInventory->Grid, SaveData->StorageRowSize, SaveData->StorageColSize);

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



void UStorageUserWidget::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{
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
		//std::swap(WorldInventoryArray, EquipInventoryArray);

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
				CreateItemInventory(EquipInventoryArray, EquipInventory->ItemSlot, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height);
				SwapSlotData(DraggingSlot, TargetSlot);
			}
		}


	}


	else
	{
		SwapSlotData(DraggingSlot, TargetSlot);
	}



}
