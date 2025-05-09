// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"

void FSlotData::SetSlotFromItemData(FItemData data)
{
	bHaveItem = true;

	Name = data.Name;
	Value = data.Value;
	Weight = data.Weight;
	Type = data.Type;
	Cost = data.Cost;
	LootDelay = data.LootDelay;
	UseDelay = data.UseDelay;
	Amount = data.Amount;
}

void FItemData::SetItemFromSlotData(FSlotData data)
{
	Name = data.Name;
	Value = data.Value;
	Weight = data.Weight;
	Type = data.Type;
	Cost = data.Cost;
	LootDelay = data.LootDelay;
	UseDelay = data.UseDelay;
	Amount = data.Amount;
}