// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


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


void UPlayerInventoryWidget::Init()
{
	if (!InventorySlots) return;

	int32 NumSlots = InventorySlots->GetChildrenCount();

	for (int32 i = 0; i < NumSlots; i++)
	{
		UWidget* SlotWidget = InventorySlots->GetChildAt(i);
		if (SlotWidget)
		{
			UInventorySlot* InventorySlot = Cast<UInventorySlot>(SlotWidget);
			InventorySlot->idx = i;
			Slots.Add(InventorySlot);
		}
	}

	Slots[1]->SetColor(-1.0f, 1.0f, 1.0f, 1.0f);
	Slots[2]->SetVisible();
}



FReply UPlayerInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return Reply.NativeReply;
}

void UPlayerInventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDragDropSlot* Operation = NewObject<UDragDropSlot>();
		OutOperation = Operation;

		if (DragWidgetClass)
		{
			UInventorySlot* DragWidget = CreateWidget<UInventorySlot>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}