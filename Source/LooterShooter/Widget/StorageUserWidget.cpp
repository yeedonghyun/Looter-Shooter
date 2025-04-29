#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (ReturnMain)
	{
		ReturnMain->OnClicked.AddDynamic(this, &UStorageUserWidget::OnReturnMainButtonClicked);
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
				CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
				SwapSlotData(DraggingSlot, TargetSlot);
			}
		}


	}


	else
	{
		SwapSlotData(DraggingSlot, TargetSlot);
	}



}
