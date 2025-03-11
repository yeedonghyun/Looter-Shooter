// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

void UInventorySlot::SetColor(float r, float g, float b, float a)
{
	IMG_Item->SetColorAndOpacity(FLinearColor(r, g, b, a));
}


void UInventorySlot::SetVisible()
{
	IMG_Item->SetVisibility(ESlateVisibility::Hidden);
}
