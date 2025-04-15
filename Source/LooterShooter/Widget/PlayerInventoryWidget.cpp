#include "PlayerInventoryWidget.h"
#include "../Save/SaveManager.h"
#include "../Inventory/Tooltip.h"

void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
	LoadInventoryData();
}

void UPlayerInventoryWidget::InitWidget()
{
	bOtherInventory = false;
	bDragging = false;

	WorldInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	WorldInventorySlot->SetVisibility(ESlateVisibility::Hidden);

	EquipInventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	EquipInventorySlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

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
	WorldInventorySlot->SetVisibility(ESlateVisibility::Hidden);
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
