#include "PlayerInventoryWidget.h"


void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bPlayerInventory = true;
	bOtherInventory = false;
	InitPlayerInventorySlots();
	IMG_OtherInventory->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerInventoryWidget::InitPlayerInventorySlots()
{
	InitInventorySlots(PlayerInventorySlots, PlayerInventorySlotArray, 0, InventoryRowSize, InventoryColSize);
}

void UPlayerInventoryWidget::InitInventorySlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize)
{
	TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'"));

	if (!InventoryClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Can't find Class")));
		return;
	}

	if (ParentSlot)
	{
		TArray<UWidget*> Rows = ParentSlot->GetAllChildren();

		if (Rows.Num() < rowSize)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Lack Row Size")));
		}

		for (int32 i = 0; i < rowSize; i++)
		{
			if (UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(Rows[i]))
			{
				for (int32 j = 0; j < colSize; j++)
				{
					if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventoryClass))
					{
						InventorySlot->InitInventorySlot((i * colSize) + j, InventoryIdx, false);
						InventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);

						HorizontalBox->AddChild(InventorySlot);
						SlotArray.Add(InventorySlot);
					}
				}
			}
		}

	}
}

void UPlayerInventoryWidget::ToggleInventory()
{
	if (!bPlayerInventory) { SetUIMode(ESlateVisibility::Visible, true, FInputModeGameAndUI()); }

	else { SetUIMode(ESlateVisibility::Hidden, false, FInputModeGameOnly()); }

	bPlayerInventory = !bPlayerInventory;
}

void UPlayerInventoryWidget::SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->bShowMouseCursor = showCursor;
		PC->SetInputMode(InData);
	}

	SetVisibility(Visible);
}


void UPlayerInventoryWidget::AddInventoryItem(AItemBase* AimedItem)
{
	for (int i = 0; i < PlayerInventorySlotArray.Num(); i++)
	{
		if (!PlayerInventorySlotArray[i]->SlotData.bHaveItem)
		{
			PlayerInventorySlotArray[i]->SetSlotFromItem(AimedItem->ItemData);
			break;
		}
	}
}

void UPlayerInventoryWidget::CreateOtherInventory(AItemBase* AimedItem)
{
	if (OtherInventorySlotArray.Num() > 0)
	{
		DeleteOtherInventory();
	}

	Bag = Cast<AItem_bag>(AimedItem);
	//AItem_bag* Bag = Cast<AItem_bag>(AimedItem);

	int32 Width = Bag->Width;
	int32 Height = Bag->Height;

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Bag Size: %d x %d"), Width, Height));

	InitInventorySlots(OtherInventorySlots, OtherInventorySlotArray, 1, Width, Height);
	bOtherInventory = true;

	TArray<FSlotData>& Items = Bag->savedItems;

	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].bHaveItem)
		{
			UInventorySlot* slot = Cast<UInventorySlot>(OtherInventorySlotArray[i]);

			slot->SetSlotFromSlot(Items[i]);
		}
	}

	SetOtherInventoryImage(Bag->ItemData.Name);
	IMG_OtherInventory->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerInventoryWidget::DeleteOtherInventory()
{
	for (int32 i = 0; i < OtherInventorySlotArray.Num(); i++)
	{
		OtherInventorySlotArray[i]->RemoveFromParent();
	}

	bOtherInventory = false;
	OtherInventorySlotArray.Empty();
	Bag = nullptr;
	IMG_OtherInventory->SetVisibility(ESlateVisibility::Hidden);
}





void UPlayerInventoryWidget::HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex)
{
	UInventorySlot* From = GetInventorySlot(FromInventorIdx, FromIndex);
	UInventorySlot* To = GetInventorySlot(ToInventoryIdx, ToIndex);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Debug::/From::%d / To::%d"), From->idx, To->idx));

	std::swap(From->SlotData.bHaveItem, To->SlotData.bHaveItem);

	std::swap(From->SlotData.Name, To->SlotData.Name);
	std::swap(From->SlotData.Value, To->SlotData.Value);
	std::swap(From->SlotData.Weight, To->SlotData.Weight);
	std::swap(From->SlotData.Type, To->SlotData.Type);

	FSlateBrush FromBrush = From->IMG_Item->Brush;
	FSlateBrush ToBrush = To->IMG_Item->Brush;
	From->IMG_Item->SetBrush(ToBrush);
	To->IMG_Item->SetBrush(FromBrush);

	To->ToggleSlot(ESlateVisibility::Visible, true);

	if(From->SlotData.bHaveItem){ From->ToggleSlot(ESlateVisibility::Visible, true); }
	else { From->ToggleSlot(ESlateVisibility::Hidden, false); }

	if (From->inventoryIdx == 1 || To->inventoryIdx == 1)
	{
		TArray<FSlotData>& Items = Bag->savedItems;

		if (From->inventoryIdx == 1)
		{
			ChangeOtherInventoryData(Items[From->idx], *From);
		}

		if (To->inventoryIdx == 1)
		{
			ChangeOtherInventoryData(Items[To->idx], *To);
		}
	}
}

void UPlayerInventoryWidget::ChangeOtherInventoryData(FSlotData& Item, const UInventorySlot& slot)
{
	Item = slot.SlotData;
}


UInventorySlot* UPlayerInventoryWidget::GetInventorySlot(int32 InventoryIdx, int32 slotIdx)
{
	switch (InventoryIdx)
	{
	case(0): return PlayerInventorySlotArray[slotIdx];
	case(1):return OtherInventorySlotArray[slotIdx];
	}
	return nullptr;
}

void UPlayerInventoryWidget::SetOtherInventoryImage(FString ItemName)
{
	FString AssetPath = FString::Format(TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Inventory/items/{0}.{0}'"), { ItemName });
	UTexture2D* NewAimImage = LoadObject<UTexture2D>(nullptr, *AssetPath);
	IMG_OtherInventory->SetBrushFromTexture(NewAimImage);
}
