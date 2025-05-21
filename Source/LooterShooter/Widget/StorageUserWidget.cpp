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

	if (ReturnMain)
	{
		ReturnMain->OnClicked.AddDynamic(this, &UStorageUserWidget::OnReturnMainButtonClicked);
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

		break;
	default:
		break;
	}

	UpdateMoney();
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
		//std::swap(WorldInventoryArray, EquipInventoryArray);

	}

	else if (DraggingSlot->SlotType != EItemType::INVENTORY && TargetSlot->SlotType == EItemType::INVENTORY)
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

	else if (DraggingSlot->SlotType == EItemType::INVENTORY && TargetSlot->SlotType != EItemType::INVENTORY)
	{
		if (TargetSlot->SlotData.Type == EItemType::INVENTORY)
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
	if (TargetSlot->SlotData.Type == EItemType::WEAPON && tradeType == ETradeType::NONE)
	{
		if (WeaponSlot)
		{
			SwapSlotData(WeaponSlot, TargetSlot);
		}
	}

	else
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