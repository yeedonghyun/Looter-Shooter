// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	UTexture2D* NewAimImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Inventory/item3.item3'"));
	IMG_Item->SetBrushFromTexture(NewAimImage);
	haveItem = true;
}

void UInventorySlot::SetTexture()
{
	UTexture2D* NewAimImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Inventory/item3.item3'"));
	IMG_Item->SetBrushFromTexture(NewAimImage);
}




void UInventorySlot::SetColor(float r, float g, float b, float a)
{
	IMG_Item->SetColorAndOpacity(FLinearColor(r, g, b, a));
}

void UInventorySlot::SetInvisible()
{
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
	haveItem = false;
}

void UInventorySlot::SetVisible()
{
	IMG_Item->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot::UpdateSlot()
{
	IMG_Item->SetVisibility(ESlateVisibility::Visible);
	haveItem = true;

	//UTexture2D* NewAimImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Assets/Image/Inventory/item3.item3'"));
	//IMG_Item->SetBrushFromTexture(NewAimImage);

}

void UInventorySlot::RequestSwap(int32 OtherSlotIdx)
{
	OnSwapRequested.Broadcast(this->idx, OtherSlotIdx);
}

//void UInventorySlot::RequestSwap(UInventorySlot* OtherSlot)
//{
//	if (OtherSlot)
//	{
//		OnSwapRequested.Broadcast(this->idx, OtherSlot->idx);
//	}
//}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (haveItem)
		{
			Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}

	return Reply.NativeReply;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		UDragDropSlot* Operation = NewObject<UDragDropSlot>();
		Operation->PrevSlotIndex = idx;
		OutOperation = Operation;

		if (DragWidgetClass)
		{
			//PlayerController* PC = GetWorld()->GetFirstPlayerController();

			UInventorySlot* DragWidget = CreateWidget<UInventorySlot>(Cast<APlayerController>(GetWorld()->GetFirstPlayerController()), DragWidgetClass);
			//UInventorySlot* DragWidget = CreateWidget<UInventorySlot>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				DragWidget->idx = idx;
				DragWidget->IMG_Item->SetBrush(IMG_Item->GetBrush());
				DragWidget->Default->SetBrush(Default->GetBrush());
				DragWidget->SetVisible();

				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Debug::%d"), DragWidget->idx));

				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragDropSlot* Operation = Cast<UDragDropSlot>(InOperation);

	if (Operation)
	{
		RequestSwap(Operation->PrevSlotIndex);
	}

	return false;
}