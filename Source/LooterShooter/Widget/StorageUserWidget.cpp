#include "StorageUserWidget.h"
#include "../Save/SaveManager.h"

void UStorageUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	tradingCost = 0;
	//money = 100;

	//USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");
	//money = SaveData->money;

	UpdateMoney();

	tradeType = ETradeType::NONE;

	if (SelectMap)
	{
		SelectMap->OnClicked.AddDynamic(this, &UStorageUserWidget::OnSelectMapButtonClicked);
	}

	if (Back)
	{
		Back->OnClicked.AddDynamic(this, &UStorageUserWidget::OnBackButtonClicked);
	}


	if (Buy)
	{
		Buy->OnClicked.AddDynamic(this, &UStorageUserWidget::OnBuyButtonClicked);
	}

	if (Undo)
	{
		Undo->OnClicked.AddDynamic(this, &UStorageUserWidget::OnUndoButtonClicked);
	}

	if (Sell)
	{
		Sell->OnClicked.AddDynamic(this, &UStorageUserWidget::OnSellButtonClicked);
	}

	if (Apply)
	{
		Apply->OnClicked.AddDynamic(this, &UStorageUserWidget::OnApplyButtonClicked);
	}

	//USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	//if (UHealth)
	//{
	//	SetHealth(SaveData->PlayerHealth / 100);
	//}

	//if (UArmor)
	//{
	//	SetArmor(SaveData->PlayerArmor / 100);
	//}

}

void UStorageUserWidget::OnBuyButtonClicked()
{
	if (tradeType == ETradeType::SELL)
	{
		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					GetBackItem(TradingInventoryArray[i]);
				}
			}
		}
	}

	else
	{
		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					TradingInventoryArray[i]->SlotData.bHaveItem = false;
					TradingInventoryArray[i]->ToggleSlot();
				}
			}
		}
	}

	tradeType = ETradeType::BUY;

	Buy->SetIsEnabled(false);
	Sell->SetIsEnabled(true);

	tradingCost = 0;
	UpdateMoney();
}

void UStorageUserWidget::OnUndoButtonClicked()
{
	if (tradeType == ETradeType::SELL)
	{
		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					GetBackItem(TradingInventoryArray[i]);
				}
			}
		}
	}

	else
	{
		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					TradingInventoryArray[i]->SlotData.bHaveItem = false;
					TradingInventoryArray[i]->ToggleSlot();
				}
			}
		}
	}


	tradeType = ETradeType::NONE;
	Buy->SetIsEnabled(true);
	Sell->SetIsEnabled(true);

	tradingCost = 0;
	UpdateMoney();
}

void UStorageUserWidget::OnSellButtonClicked()
{
	tradeType = ETradeType::SELL;

	Buy->SetIsEnabled(true);
	Sell->SetIsEnabled(false);

	if (TradingInventory)
	{
		for (int i = 0; i < TradingInventoryArray.Num(); i++)
		{
			if (TradingInventoryArray[i]->SlotData.bHaveItem)
			{
				TradingInventoryArray[i]->SlotData.bHaveItem = false;
				TradingInventoryArray[i]->ToggleSlot();
			}
		}
	}
	tradingCost = 0;
	UpdateMoney();
}

void UStorageUserWidget::OnApplyButtonClicked()
{
	switch (tradeType)
	{
	case ETradeType::NONE:
		break;
	case ETradeType::BUY:

		if (money - tradingCost < 0)
		{
			ShowWarningMessage("Not enogh money");
			return;
		}


		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					GetBackItem(TradingInventoryArray[i]);
				}
			}
		}

		money -= tradingCost;
		tradingCost = 0;
		SaveInventories();

		break;
	case ETradeType::SELL:

		if (TradingInventory)
		{
			for (int i = 0; i < TradingInventoryArray.Num(); i++)
			{
				if (TradingInventoryArray[i]->SlotData.bHaveItem)
				{
					TradingInventoryArray[i]->SlotData.bHaveItem = false;
					TradingInventoryArray[i]->ToggleSlot();
				}
			}
		}

		money += tradingCost;
		tradingCost = 0;
		SaveInventories();

		break;
	default:
		break;
	}

	UpdateMoney();
}



void UStorageUserWidget::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{
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

			//if (ToInventory->ItemSlot->UnderInventoryType == EUnderInventoryType::WORLDBAG)
			//{
			//	DeleteWorldInventory();
			//}

			bHaveEquipInventory = false;
		}

	}

	else if (DraggingSlot->SlotType == EItemType::INVENTORY && TargetSlot->SlotType != EItemType::INVENTORY)
	{
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

	//else if (DraggingSlot->SlotData.Type == EItemType::AMMO && TargetSlot->SlotData.Type == EItemType::AMMO
	//	)
	//{
	//	int NeedAmmo = DraggingSlot->SlotData.MaxAmount - DraggingSlot->SlotData.Amount;
	//	int maxBringable = FMath::Min(NeedAmmo, TargetSlot->SlotData.Amount);

	//	DraggingSlot->SlotData.Amount += maxBringable;
	//	TargetSlot->SlotData.Amount -= maxBringable;

	//	DraggingSlot->ToggleSlot();

	//	if (TargetSlot->SlotData.Amount == 0)
	//	{
	//		TargetSlot->SlotData.bHaveItem = false;
	//	}

	//	TargetSlot->ToggleSlot();

	//}

	else
	{
		if (TargetSlot->UnderInventoryType != EUnderInventoryType::TRADE && TargetSlot->UnderInventoryType != EUnderInventoryType::SHOP
			&& DraggingSlot->UnderInventoryType != EUnderInventoryType::TRADE && DraggingSlot->UnderInventoryType != EUnderInventoryType::SHOP
			)
		{
			SwapSlotData(DraggingSlot, TargetSlot);

		}


	}

	SaveInventories();
}


void UStorageUserWidget::MoveItemTradeInventory(UInventorySlot* TargetSlot)
{
	for (int i = 0; i < TradingInventoryArray.Num(); i++)
	{
		if (!TradingInventoryArray[i]->SlotData.bHaveItem)
		{
			TradingInventoryArray[i]->SetSlotFromSlot(TargetSlot->SlotData);
			tradingCost += TargetSlot->SlotData.Cost;
			break;
		}
	}
}



void UStorageUserWidget::HandleSlotRightClickRequest(UInventorySlot* TargetSlot)
{
	if (TargetSlot->SlotData.Type == EItemType::WEAPON)
	{
		if (TargetSlot->UnderInventoryType != EUnderInventoryType::TRADE && TargetSlot->UnderInventoryType != EUnderInventoryType::SHOP)
		{
			if (WeaponSlot)
			{
				SwapSlotData(WeaponSlot, TargetSlot);
			}
		}
	}




	//if (TargetSlot->SlotData.Type == EItemType::WEAPON)
	//{
	//	if (TargetSlot->UnderInventoryType == EUnderInventoryType::PLAYER ||
	//		TargetSlot->UnderInventoryType == EUnderInventoryType::EQUIP ||
	//		TargetSlot->UnderInventoryType == EUnderInventoryType::STORAGE)
	//	{
	//		if (WeaponSlot)
	//		{
	//			SwapSlotData(WeaponSlot, TargetSlot);
	//		}
	//	}
	//}



	//if (TargetSlot->SlotData.Type == EItemType::WEAPON && tradeType == ETradeType::NONE
	//	&& (TargetSlot->UnderInventoryType == EUnderInventoryType::PLAYER || 
	//		TargetSlot->UnderInventoryType == EUnderInventoryType::EQUIP ||
	//		TargetSlot->UnderInventoryType == EUnderInventoryType::STORAGE)
	//	)
	//{
	//	if (WeaponSlot)
	//	{
	//		SwapSlotData(WeaponSlot, TargetSlot);
	//	}
	//}

	//else
	{

		switch (TargetSlot->UnderInventoryType)
		{
		case EUnderInventoryType::SHOP:

			if (tradeType == ETradeType::BUY)
			{
				if (TradingInventory)
				{
					MoveItemTradeInventory(TargetSlot);
					break;
				}
			}


			break;

		case EUnderInventoryType::TRADE:

			if (tradeType == ETradeType::SELL)
			{
				tradingCost -= TargetSlot->SlotData.Cost;
				GetBackItem(TargetSlot);
			}

			else
			{
				tradingCost -= TargetSlot->SlotData.Cost;
				TargetSlot->SlotData.bHaveItem = false;
				TargetSlot->ToggleSlot();
			}

			break;

		case EUnderInventoryType::PLAYER:
			if (tradeType == ETradeType::SELL)
			{
				if (TradingInventory)
				{
					MoveItemTradeInventory(TargetSlot);
					TargetSlot->SlotData.bHaveItem = false;
					TargetSlot->ToggleSlot();
					break;
				}
			}

			break;

		case EUnderInventoryType::STORAGE:
			if (tradeType == ETradeType::SELL)
			{
				if (TradingInventory)
				{
					MoveItemTradeInventory(TargetSlot);
					TargetSlot->SlotData.bHaveItem = false;
					TargetSlot->ToggleSlot();
					break;
				}
			}
			break;

		case EUnderInventoryType::EQUIP:
			if (tradeType == ETradeType::SELL)
			{
				if (TradingInventory)
				{
					MoveItemTradeInventory(TargetSlot);
					TargetSlot->SlotData.bHaveItem = false;
					TargetSlot->ToggleSlot();
					break;
				}
			}
			break;

		default:
			break;
		}

		UpdateMoney();
	}

	SaveInventories();
}


void UStorageUserWidget::GetBackItem(UInventorySlot* TargetSlot)
{
	if (StorageInventory)
	{
		for (int i = 0; i < StorageInventoryArray.Num(); i++)
		{
			if (!StorageInventoryArray[i]->SlotData.bHaveItem)
			{
				StorageInventoryArray[i]->SetSlotFromSlot(TargetSlot->SlotData);
				TargetSlot->SlotData.bHaveItem = false;
				TargetSlot->ToggleSlot();
			}
		}
	}

	if (PlayerInventory)
	{
		for (int i = 0; i < PlayerInventoryArray.Num(); i++)
		{
			if (!PlayerInventoryArray[i]->SlotData.bHaveItem)
			{
				PlayerInventoryArray[i]->SetSlotFromSlot(TargetSlot->SlotData);
				TargetSlot->SlotData.bHaveItem = false;
				TargetSlot->ToggleSlot();
			}
		}
	}

	if (EquipInventory)
	{
		for (int i = 0; i < EquipInventoryArray.Num(); i++)
		{
			if (!EquipInventoryArray[i]->SlotData.bHaveItem)
			{
				EquipInventoryArray[i]->SetSlotFromSlot(TargetSlot->SlotData);
				TargetSlot->SlotData.bHaveItem = false;
				TargetSlot->ToggleSlot();
			}
		}
	}
}



void UStorageUserWidget::UpdateMoney()
{
	FString TradingCostString = TEXT(" $ ") + FString::FromInt(tradingCost);
	TradingCost->SetText(FText::FromString(TradingCostString));

	FString moneyString = TEXT(" $ ") + FString::FromInt(money);
	Money->SetText(FText::FromString(moneyString));
}


void UStorageUserWidget::OnSelectMapButtonClicked()
{
	if (!WeaponSlot->SlotData.bHaveItem)
	{
		if (InventoryWarningMessage)
		{
			ShowWarningMessage("No weapon");
		}

		return;
	}

	SaveInventories();

	this->RemoveFromParent();

	if (TSubclassOf<UUserWidget> SelectMapWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_SelectMapUserWidget.BP_SelectMapUserWidget_C'")))
	{
		if (UUserWidget* SelectMapWidget = CreateWidget<UUserWidget>(GetWorld(), SelectMapWidgetClass))
		{
			SelectMapWidget->AddToViewport();
		}
	}
}



void UStorageUserWidget::OnBackButtonClicked()
{
	this->RemoveFromParent();

	if (TSubclassOf<UUserWidget> StorageWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_MainMenuUserWidget.BP_MainMenuUserWidget_C'")))
	{
		UUserWidget* StorageWidget = CreateWidget<UUserWidget>(GetWorld(), StorageWidgetClass);
		if (StorageWidget)
		{
			StorageWidget->AddToViewport();
		}
	}
}
