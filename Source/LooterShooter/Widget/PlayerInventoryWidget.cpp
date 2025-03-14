// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryWidget.h"



void UPlayerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Init();
}

void UPlayerInventoryWidget::OpenInventory()
{
	SetVisibility(ESlateVisibility::Visible);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->bShowMouseCursor = true; 
		PC->SetInputMode(FInputModeGameAndUI());
		//PC->SetInputMode(FInputModeUIOnly());
	}
}

void UPlayerInventoryWidget::CloseInventory()
{
	SetVisibility(ESlateVisibility::Hidden);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly()); 
	}
}


void UPlayerInventoryWidget::AddItem(AItemBase& item)
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (!Slots[i]->haveItem)
		{
			Slots[i]->itemType = item.GetItemType();
			Slots[i]->UpdateSlot();
			break;
		}
	}
}


void UPlayerInventoryWidget::Init()
{
	int32 rowMax = 2;
	int32 colMax = 5;

	TArray<UWidget*> rows = PlayerSlot->GetAllChildren();

	for (int32 i = 0; i < rowMax; i++)
	{
		UHorizontalBox* HorizontalBox = Cast<UHorizontalBox>(rows[i]);

		for (int32 j = 0; j < colMax; j++)
		{
			if (TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Inventory/BP_InventorySlot.BP_InventorySlot_C'")))
			{
				UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(GetWorld(), InventoryClass);

				if (InventorySlot)
				{
					InventorySlot->AddToViewport();
					InventorySlot->idx = (i * colMax) + j;
					HorizontalBox->AddChild(InventorySlot);
					Slots.Add(InventorySlot);
					InventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
				}
			}
		}
	}






	//int32 NumSlots = InventorySlots->GetChildrenCount();

	//for (int32 i = 0; i < NumSlots; i++)
	//{
	//	UWidget* SlotWidget = InventorySlots->GetChildAt(i);
	//	if (SlotWidget)
	//	{
	//		UInventorySlot* InventorySlot = Cast<UInventorySlot>(SlotWidget);
	//		InventorySlot->idx = i;
	//		Slots.Add(InventorySlot);

	//		InventorySlot->OnSwapRequested.AddUObject(this, &UPlayerInventoryWidget::HandleSwapRequest);
	//	}
	//}

	for (int32 i = 0; i < rowMax * colMax; i++)
	{
		Slots[i]->SetInvisible();
	}
}

void UPlayerInventoryWidget::HandleSwapRequest(int32 FromIndex, int32 ToIndex)
{
	if (!Slots[FromIndex]->haveItem)
	{
		Slots[FromIndex]->UpdateSlot();
		Slots[ToIndex]->SetInvisible();
	}

	//else
	//{
	//	UInventorySlot* Operation = NewObject<UInventorySlot>();

	//	Operation = Slots[ToIndex];
	//	Slots[ToIndex] = Slots[FromIndex];
	//	Slots[FromIndex] = Operation;
	//}

	//UE_LOG(LogTemp, Log, TEXT("Swapped Slot %d with Slot %d"), FromIndex, ToIndex);
}