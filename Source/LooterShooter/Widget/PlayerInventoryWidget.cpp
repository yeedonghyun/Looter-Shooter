#include "PlayerInventoryWidget.h"
#include "../Save/SaveManager.h"
#include "../Inventory/Tooltip.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//bInventoryActive = true;
	bOtherInventory = false;
	bDragging = false;

	WorldInventorySlot->InitIdxes(-1, 1);
	WorldInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	WorldInventorySlot->SetVisibility(ESlateVisibility::Hidden);

	EquipInventorySlot->InitIdxes(-1, 2);
	EquipInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	EquipInventorySlot->SetVisibility(ESlateVisibility::Visible);

	CreateSlots(PlayerInventory, PlayerInventoryArray, 0, InventoryRowSize, InventoryColSize);
	LoadSelcectSlotData(PlayerInventoryArray, "InventoryItems");

	if (bEquipInventory)
	{
		if (TSubclassOf<AItem_bag> TestItemClass = LoadClass<AItem_bag>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/BP_Item_bag.BP_Item_bag_C'"))) // 나중에 storarge 형태등으로
		{
			AItem_bag* DefaultBag = TestItemClass->GetDefaultObject<AItem_bag>();
			CreateSlots(EquipInventory, EquipInventoryArray, 2, DefaultBag->Width, DefaultBag->Height);
			EquipInventorySlot->SetSlotFromItem(DefaultBag->ItemData);
		}

		LoadSelcectSlotData(EquipInventoryArray, "PlayerBag");
	}


	if (TSubclassOf<UUserWidget> ToolTip = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Inventory/BP_Tooltip.BP_Tooltip_C'")))
	{
		SlotToolTip = CreateWidget<UTooltip>(GetWorld(), ToolTip);

		if (SlotToolTip)
		{
			SlotToolTip->AddToViewport(999);
		}
	}

	SlotToolTip->SetVisibility(ESlateVisibility::Hidden);


	FString Info = FString::Printf(
		TEXT("Health : %d\nArmor : %d\n"),
		5,
		4
	);

	PlayerStatus->SetText(FText::FromString(Info));
}


void UPlayerInventoryWidget::AddItemEmptySlot(AItemBase* AimedItem)
{
	int emptyIdx = FindEmptySlot(PlayerInventoryArray);
	PlayerInventoryArray[emptyIdx]->SetSlotFromItem(AimedItem->ItemData);

	SaveSelectSlotData(PlayerInventoryArray, "Inventory");
}

void UPlayerInventoryWidget::CreateWorldInventory(AItemBase* AimedItem)
{
	if (WorldInventoryArray.Num() > 0)
	{
		DeleteWorldInventory();
	}

	Bag = Cast<AItem_bag>(AimedItem);
	//AItem_bag* Bag = Cast<AItem_bag>(AimedItem);
	CreateSlots(WorldInventory, WorldInventoryArray, 1, Bag->Width, Bag->Height);
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

	WorldInventorySlot->SetVisibility(ESlateVisibility::Visible);
	WorldInventorySlot->SetSlotFromItem(Bag->ItemData);
}

void UPlayerInventoryWidget::DeleteWorldInventory()
{
	for (int32 i = 0; i < WorldInventoryArray.Num(); i++)
	{
		WorldInventoryArray[i]->RemoveFromParent();
	}

	WorldInventoryArray.Empty();
	bOtherInventory = false;
	WorldInventorySlot->SetVisibility(ESlateVisibility::Hidden);

}



UInventorySlot* UPlayerInventoryWidget::GetInventorySlot(int32 InventoryIdx, int32 slotIdx)
{
	switch (InventoryIdx)
	{
	case(0): return PlayerInventoryArray[slotIdx];
	case(1):return WorldInventoryArray[slotIdx];
	case(2):return EquipInventoryArray[slotIdx];
	}
	return nullptr;
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


void UPlayerInventoryWidget::HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex)
{
	if (FromIndex == -1 || ToIndex == -1)
	{
		if (FromIndex == -1 && ToIndex == -1 && FromInventorIdx != ToInventoryIdx)
		{
			for (int i = 0; i < EquipInventoryArray.Num(); i++)
			{
				SwapSlot(EquipInventoryArray[i], WorldInventoryArray[i]);
			}
		}
	}

	else
	{
		UInventorySlot* From = GetInventorySlot(FromInventorIdx, FromIndex);
		UInventorySlot* To = GetInventorySlot(ToInventoryIdx, ToIndex);
		SwapSlot(From, To);
	}

	SaveSelectSlotData(PlayerInventoryArray, "Inventory");

	TArray<FSlotData>& Items = Bag->savedItems;

	for (int i = 0; i < WorldInventoryArray.Num(); i++)
	{
		Items[i] = WorldInventoryArray[i]->SlotData;
	}


}
