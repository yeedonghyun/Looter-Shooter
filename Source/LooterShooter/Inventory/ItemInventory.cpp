// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventory.h"

void UItemInventory::NativeConstruct()
{
	Super::NativeConstruct();

	Name->SetText(FText::FromString(InventoryName));
}