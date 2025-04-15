#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
	LoadInventoryData();

	//if (ReturnMain)
	//{
	//	ReturnMain->OnClicked.AddDynamic(this, &UStorageUserWidget::OnReturnMainButtonClicked);
	//}

}

void UStorageUserWidget::InitWidget()
{
	bDragging = false;

	//WorldInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	//WorldInventorySlot->SetVisibility(ESlateVisibility::Hidden);

	//EquipInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	EquipInventorySlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

	//ArmorSlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
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

	CreateSlots(PlayerInventory, PlayerInventoryArray, 0, SaveData->InventoryRowSize, SaveData->InventoryColSize);

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
			CreateSlots(EquipInventory, EquipInventoryArray, 2, DefaultBag->Width, DefaultBag->Height);
			EquipInventorySlot->SetSlotFromItem(DefaultBag->ItemData);
		}

		if (SaveData->EquipInventoryItems.Num() != 0)
		{
			SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
		}
	}

	PlayerHealth = SaveData->PlayerHealth;
	PlayerArmor = SaveData->PlayerArmor;

	FString Info = FString::Printf(TEXT("Health : %d\nArmor : %d\n"),
		PlayerHealth,
		PlayerArmor
	);

	PlayerStatus->SetText(FText::FromString(Info));
}


void UStorageUserWidget::OnReturnMainButtonClicked()
{
	//this->RemoveFromParent();

	//if (TSubclassOf<UUserWidget> SelectMapWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_SelectMapUserWidget.BP_SelectMapUserWidget_C'")))
	//{
	//	UUserWidget* SelectMapWidget = CreateWidget<UUserWidget>(GetWorld(), SelectMapWidgetClass);
	//	if (SelectMapWidget)
	//	{
	//		SelectMapWidget->AddToViewport();
	//	}
	//}

	//TArray<FSlotData> InventoryItems;

	//for (int i = 0; i < PlayerInventorySlotArray.Num(); i++)
	//{
	//	InventoryItems.Add(PlayerInventorySlotArray[i]->SlotData);
	//}

	////USaveManager::SaveSelectData(InventoryItems, "Save1", "Inventory");

	//TArray<FSlotData> StaorageItems;

	//for (int i = 0; i < StorageSlotArray.Num(); i++)
	//{
	//	StaorageItems.Add(StorageSlotArray[i]->SlotData);
	//}

	////USaveManager::SaveSelectData(StaorageItems, "Save1", "Storage");
}