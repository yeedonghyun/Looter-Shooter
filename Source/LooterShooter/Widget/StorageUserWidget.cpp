#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitSlots(StorageSlots, StorageSlotArray,1, RowSize, ColSize);
	InitSlots(InventorySlots, PlayerInventorySlotArray,0, RowSize, ColSize);

	//TArray<FSlotData> Items = USaveManager::LoadInventoryItems("Save1");

	//if (Items.Num() != 0)
	//{
	//	for (int32 i = 0; i < Items.Num(); i++)
	//	{
	//		if (Items[i].bHaveItem)
	//		{
	//			PlayerInventorySlotArray[i]->SetSlotFromSlot(Items[i]);
	//		}
	//	}
	//}

	//Items = USaveManager::LoadSelectData("Save1","Storage");

	//if (Items.Num() != 0)
	//{
	//	for (int32 i = 0; i < Items.Num(); i++)
	//	{
	//		if (Items[i].bHaveItem)
	//		{
	//			StorageSlotArray[i]->SetSlotFromSlot(Items[i]);
	//		}
	//	}
	//}



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
		UUserWidget* SelectMapWidget = CreateWidget<UUserWidget>(GetWorld(), SelectMapWidgetClass);
		if (SelectMapWidget)
		{
			SelectMapWidget->AddToViewport();
		}
	}

	TArray<FSlotData> InventoryItems;

	for (int i = 0; i < PlayerInventorySlotArray.Num(); i++)
	{
		InventoryItems.Add(PlayerInventorySlotArray[i]->SlotData);
	}

	//USaveManager::SaveSelectData(InventoryItems, "Save1", "Inventory");

	TArray<FSlotData> StaorageItems;

	for (int i = 0; i < StorageSlotArray.Num(); i++)
	{
		StaorageItems.Add(StorageSlotArray[i]->SlotData);
	}

	//USaveManager::SaveSelectData(StaorageItems, "Save1", "Storage");
}


void UStorageUserWidget::InitSlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize)
{
	TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'"));

	if (!InventoryClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Can't find Class")));
		return;
	}

	if (ParentSlot)
	{
		for (int32 i = 0; i < rowSize; i++)
		{
			// 새로운 가로 박스 생성
			if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(ParentSlot))
			{
				ParentSlot->AddChild(HorizontalBox);

				for (int32 j = 0; j < colSize; j++)
				{
					if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventoryClass))
					{
						InventorySlot->InitInventorySlot((i * colSize) + j, InventoryIdx, false, i, j);
						InventorySlot->OnSwapRequested.AddUObject(this, &UStorageUserWidget::HandleSwapRequest);

						HorizontalBox->AddChild(InventorySlot);
						SlotArray.Add(InventorySlot);
					}
				}
			}
		}

	}
}


void UStorageUserWidget::HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex)
{
	UInventorySlot* From = GetInventorySlot(FromInventorIdx, FromIndex);
	UInventorySlot* To = GetInventorySlot(ToInventoryIdx, ToIndex);

	SwapSlot(From, To);
}

UInventorySlot* UStorageUserWidget::GetInventorySlot(int32 InventoryIdx, int32 slotIdx)
{
	switch (InventoryIdx)
	{
	case(0): return PlayerInventorySlotArray[slotIdx];
	case(1):return StorageSlotArray[slotIdx];
	}
	return nullptr;
}

void UStorageUserWidget::ChangeOtherInventoryData(FSlotData& Item, const UInventorySlot& slot)
{
	Item = slot.SlotData;
}