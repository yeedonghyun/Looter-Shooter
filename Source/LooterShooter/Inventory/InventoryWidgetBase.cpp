// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"


void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	bDragging = false;
	bHaveEquipInventory = false;
	bOpenRightClickOption = false;
	bClickDetectionEnabled = false;
	sumAmmo = 0;

	InitWidget();

	if (TSubclassOf<UUserWidget> ToolTip = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Inventory/BP_Tooltip.BP_Tooltip_C'")))
	{
		SlotToolTip = CreateWidget<UTooltip>(GetWorld(), ToolTip);

		if (SlotToolTip)
		{
			SlotToolTip->AddToViewport(999);
			SlotToolTip->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	ToggleWarningMessage(false);
}


void UInventoryWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 첫 프레임 이후 클릭 감지 활성화
	if (bOpenRightClickOption)
	{
		bClickDetectionEnabled = true;
	}
}

void UInventoryWidgetBase::InitWidget()
{
	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	if (PlayerInventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerInventory")));

		CreateInventory(PlayerInventoryArray, PlayerInventory->Grid, SaveData->InventoryRowSize, SaveData->InventoryColSize, EUnderInventoryType::PLAYER);

		if (SaveData->InventoryItems.Num() != 0)
		{
			SetArrayData(PlayerInventoryArray, SaveData->InventoryItems);
		}
	}

	if (EquipInventory)
	{
		EquipInventory->ItemSlot->UnderInventoryType = EUnderInventoryType::EQUIP;
		EquipInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
		EquipInventory->ItemSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);


		if (SaveData->bEquipInventory)
		{
			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *SaveData->EquipInventoryName, *SaveData->EquipInventoryName);

			if (TSubclassOf<AItem_Inventory> ItemClass = LoadClass<AItem_Inventory>(nullptr, *FullPath))
			{
				AItem_Inventory* DefaultBag = ItemClass->GetDefaultObject<AItem_Inventory>();
				CreateInventory(EquipInventoryArray, EquipInventory->Grid, DefaultBag->Width, DefaultBag->Height, EUnderInventoryType::EQUIP);
				EquipInventory->ItemSlot->SetSlotFromItem(DefaultBag->ItemData);
			}

			if (SaveData->EquipInventoryItems.Num() != 0)
			{
				SetArrayData(EquipInventoryArray, SaveData->EquipInventoryItems);
			}
		}


	}

	if (StorageInventory)
	{
		CreateInventory(StorageInventoryArray, StorageInventory->Grid, SaveData->StorageRowSize, SaveData->StorageColSize, EUnderInventoryType::STORAGE);

		if (SaveData->StorageItems.Num() != 0)
		{
			SetArrayData(StorageInventoryArray, SaveData->StorageItems);
		}
	}

	if (ShopInventory)
	{
		CreateInventory(ShopInventoryArray, ShopInventory->Grid, 2, 5, EUnderInventoryType::SHOP);

		TArray<FString> itemList = { "Ammo1" , "Armor1" ,"Armor3","Syringe" ,"Medikit" };

		for (int i = 0; i < 5; i++)
		{
			FString name = itemList[i];
			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *name, *name);

			if (TSubclassOf<AItemBase> ItemClass = LoadClass<AItemBase>(nullptr, *FullPath))
			{
				AItemBase* DefaultObject = ItemClass->GetDefaultObject<AItemBase>();

				if (DefaultObject)
				{
					ShopInventoryArray[i]->SetSlotFromItem(DefaultObject->ItemData);
				}
			}
		}

	}

	if (TradingInventory)
	{
		CreateInventory(TradingInventoryArray, TradingInventory->Grid, 10, 5, EUnderInventoryType::TRADE);
	}








	if (WorldInventory)
	{
		WorldInventory->ItemSlot->UnderInventoryType = EUnderInventoryType::WORLDBAG;
		WorldInventory->ItemSlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
		WorldInventory->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (WorldBoxInventory)
	{
		WorldBoxInventory->SetVisibility(ESlateVisibility::Collapsed);
	}

	money = SaveData->money;

	if (WeaponSlot)
	{
		WeaponSlot->UnderInventoryType = EUnderInventoryType::NONE;
		WeaponSlot->IMG_Item->SetVisibility(ESlateVisibility::Hidden);

		if (SaveData->bEquipWeapon)
		{
			FString name = SaveData->EquipWeaponName;
			FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *name, *name);

			if (TSubclassOf<AItemBase> ItemClass = LoadClass<AItemBase>(nullptr, *FullPath))
			{
				AItemBase* DefaultObject = ItemClass->GetDefaultObject<AItemBase>();

				if (DefaultObject)
				{
					WeaponSlot->SetSlotFromItem(DefaultObject->ItemData);
				}
			}

		}

	}
}



void UInventoryWidgetBase::CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize, EUnderInventoryType InventoryType)
{
	if (TSubclassOf<UUserWidget> InventorySlotClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'")))
	{
		if (ParentGrid)
		{
			SlotArray.Empty();

			for (int32 i = 0; i < rowSize; i++)
			{
				if (UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(ParentGrid))
				{
					ParentGrid->AddChild(HorizontalBox);

					for (int32 j = 0; j < colSize; j++)
					{
						if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass))
						{
							InventorySlot->CreateInventorySlot((i * colSize) + j, EItemType::STUFF, InventoryType);
							InventorySlot->OnSwapRequested.AddUObject(this, &UInventoryWidgetBase::HandleSwapRequest);
							InventorySlot->OnSlotActionRequested.AddUObject(this, &UInventoryWidgetBase::HandleSlotActionRequest);
							HorizontalBox->AddChild(InventorySlot);

							SlotArray.Add(InventorySlot);
						}
					}
				}
			}
		}
	}
}



void UInventoryWidgetBase::SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData)
{
	for (int32 i = 0; i < SlotArray.Num(); i++)
	{
		if (ArrayData[i].bHaveItem)
		{
			SlotArray[i]->SetSlotFromSlot(ArrayData[i]);
		}
	}
}

void UInventoryWidgetBase::SwapSlotData(UInventorySlot*& DraggingSlot, UInventorySlot*& TargetSlot)
{
	std::swap(DraggingSlot->SlotData, TargetSlot->SlotData);

	FSlateBrush FromBrush = DraggingSlot->IMG_Item->Brush;
	FSlateBrush ToBrush = TargetSlot->IMG_Item->Brush;

	DraggingSlot->IMG_Item->SetBrush(ToBrush);
	TargetSlot->IMG_Item->SetBrush(FromBrush);

	TargetSlot->ToggleSlot();
	DraggingSlot->ToggleSlot();
}



void UInventoryWidgetBase::HandleSlotActionRequest(UInventorySlot* TargetSlot, ESlotActionType type, bool bActive)
{


	switch (type)
	{
	case ESlotActionType::DROP:

		if (bOpenRightClickOption)
		{
			bOpenRightClickOption = false;
			bClickDetectionEnabled = false;
			RightClickOption->RemoveFromParent();
		}

		DedicateSlot = TargetSlot;

		CreateClickOption();



		break;

	case ESlotActionType::USE:

		if (InventoryWidgetType == EInventoryWidgetType::STORAGE)
		{
			HandleSlotRightClickRequest(TargetSlot);
		}

		else
		{
			if (bOpenRightClickOption)
			{
				bOpenRightClickOption = false;
				bClickDetectionEnabled = false;
				RightClickOption->RemoveFromParent();
			}

			DedicateSlot = TargetSlot;

			CreateClickOption();
		}


		break;

	case ESlotActionType::CHECK:
		if (!bDragging && SlotToolTip)
		{
			SlotToolTip->UpdateTexts(TargetSlot->SlotData, bActive);
		}
		break;

	case ESlotActionType::DRAG:
		bDragging = bActive;
		if (bDragging && SlotToolTip)
		{
			SlotToolTip->UpdateTexts(TargetSlot->SlotData, !bActive);
		}

		break;

	default:
		break;
	}

}

void UInventoryWidgetBase::CreateClickOption()
{
	bOpenRightClickOption = true;

	if (TSubclassOf<URightClickOption> RightClickClass = LoadClass<URightClickOption>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_RightClickOption.BP_RightClickOption_C'")))
	{
		RightClickOption = CreateWidget<URightClickOption>(GetWorld(), RightClickClass);

		if (RightClickOption)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreateWidget")));

			FVector2D curMousePosition;

			if (GetWorld()->GetFirstPlayerController()->GetMousePosition(curMousePosition.X, curMousePosition.Y))
			{
				RightClickOption->SetPositionInViewport(curMousePosition + FVector2D(2.0f, 0.0f));
			}

			RightClickOption->AddToViewport();
			RightClickOption->OnActionSelected.AddDynamic(this, &UInventoryWidgetBase::OnSlotActionSelected);

			if (DedicateSlot)
			{
				if (DedicateSlot->SlotData.Type == EItemType::AMMO || DedicateSlot->SlotData.Type == EItemType::INVENTORY)
				{
					RightClickOption->BUse->SetIsEnabled(false);
				}

				else
				{
					RightClickOption->BUse->SetIsEnabled(true);
				}
			}

		}
	}

}


void UInventoryWidgetBase::HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot)
{

}

void UInventoryWidgetBase::HandleSlotRightClickRequest(UInventorySlot* TargetSlot)
{
}


void UInventoryWidgetBase::UseItem(UInventorySlot* TargetSlot)
{
}

void UInventoryWidgetBase::DropItem(UInventorySlot* TargetSlot)
{
}

void UInventoryWidgetBase::UpdateMagazine()
{
}


void UInventoryWidgetBase::SaveInventories()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Save")));

	USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");

	if (PlayerInventory)
	{
		TArray<FSlotData> tmp;
		for (int i = 0; i < PlayerInventoryArray.Num(); i++)
		{
			tmp.Add(PlayerInventoryArray[i]->SlotData);
		}
		SaveData->InventoryItems = tmp;
	}

	if (EquipInventory)
	{
		SaveData->bEquipInventory = bHaveEquipInventory;

		if (bHaveEquipInventory)
		{
			SaveData->EquipInventoryName = EquipInventory->ItemSlot->SlotData.Name;

			TArray<FSlotData> tmp;
			for (int i = 0; i < EquipInventoryArray.Num(); i++)
			{
				tmp.Add(EquipInventoryArray[i]->SlotData);
			}

			SaveData->EquipInventoryItems = tmp;
		}

		else
		{
			SaveData->EquipInventoryName = "";
		}
	}

	if (StorageInventory)
	{
		TArray<FSlotData> tmp;
		for (int i = 0; i < StorageInventoryArray.Num(); i++)
		{
			tmp.Add(StorageInventoryArray[i]->SlotData);
		}

		SaveData->StorageItems = tmp;
	}
	
	SaveData->money = money;

	USaveManager::SaveDataSet("Save1", SaveData);
}

TArray<UInventorySlot*>& UInventoryWidgetBase::ReturnInventoryArray(EUnderInventoryType InventoryType)
{
	switch (InventoryType)
	{
	case EUnderInventoryType::PLAYER: return PlayerInventoryArray;
	case EUnderInventoryType::EQUIP: return EquipInventoryArray;
	case EUnderInventoryType::WORLDBAG: return WorldBagInventoryArray;
	case EUnderInventoryType::WORLDBOX: return WorldBoxInventoryArray;
	case EUnderInventoryType::STORAGE: return StorageInventoryArray;

	}

	return PlayerInventoryArray;
}



void UInventoryWidgetBase::ShowWarningMessage(FString fs)
{
	if (InventoryWarningMessage)
	{
		if (UpdateHandle.IsValid())
		{
			UpdateHandle.Invalidate();
		}

		GetWorld()->GetTimerManager().SetTimer(UpdateHandle, FTimerDelegate::CreateLambda([this]()
			{
				ToggleWarningMessage(false);
			}), 1.f, false);


		ToggleWarningMessage(true);
		InventoryWarningMessage->SetText(FText::FromString(fs));
	}
}

void UInventoryWidgetBase::ToggleWarningMessage(bool bActive)
{
	if (InventoryWarningMessage)
	{
		if (bActive)
		{
			InventoryWarningMessage->SetVisibility(ESlateVisibility::Visible);
		}
		
		else InventoryWarningMessage->SetVisibility(ESlateVisibility::Collapsed);
	}
}


void UInventoryWidgetBase::OnSlotActionSelected(ESlotActionType ActionType)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("OnSlotAction")));

	if (DedicateSlot)
	{
		switch (ActionType)
		{
		case ESlotActionType::USE:

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("OnSlotActionSelectedUseItem")));

			UseItem(DedicateSlot);
			break;
		case ESlotActionType::DROP:
			DropItem(DedicateSlot);
			break;
		default:
			break;
		}
	}

	bOpenRightClickOption = false;
	bClickDetectionEnabled = false;
	RightClickOption->RemoveFromParent();
}

FReply UInventoryWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bClickDetectionEnabled)
	{
		return FReply::Unhandled(); // 아직 감지 비활성화
	}

	if (RightClickOption && !RightClickOption->IsHovered())
	{
		RightClickOption->RemoveFromParent();
		bOpenRightClickOption = false;
		bClickDetectionEnabled = false;
	}


	return FReply::Handled(); // 클릭 소모
}

void UInventoryWidgetBase::CheckAmmo()
{
	bool bFindAmmo = false;
	sumAmmo = 0;

	if (PlayerInventory)
	{
		for (int32 i = 0; i < PlayerInventoryArray.Num(); i++)
		{
			if (PlayerInventoryArray[i]->SlotData.bHaveItem)
			{
				if (PlayerInventoryArray[i]->SlotData.Type == EItemType::AMMO)
				{
					if (!bFindAmmo)
					{
						FirstAmmoSlot = PlayerInventoryArray[i];
						bFindAmmo = true;
					}

					sumAmmo += PlayerInventoryArray[i]->SlotData.Amount;
				}
			}
		}

	}

	if(bHaveEquipInventory)
	{
		if (EquipInventory)
		{

		}
	}

}

void UInventoryWidgetBase::UpdateAmmo()
{
	sumAmmo -= 1;
	FirstAmmoSlot->SlotData.Amount -= 1;

	if (FirstAmmoSlot->SlotData.Amount == 0)
	{
		FirstAmmoSlot->SlotData.bHaveItem = false;
		FirstAmmoSlot->ToggleSlot();
		CheckAmmo();
	}

	//텍스트 업데이트
}