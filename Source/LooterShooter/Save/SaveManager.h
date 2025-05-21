// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Item/ItemData.h"

#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API USaveManager : public USaveGame
{
	GENERATED_BODY()
	

public:

	USaveManager();
	static USaveManager* GetSaveInstance(const FString& SaveSlotName);
	//static void SaveData(const FString& SaveSlotName);

	static void CreateNewSaveData(const FString& SaveSlotName);

	static void SaveDataSet(const FString& SaveSlotName, USaveManager* SaveGameInstance);

	int32 InventoryRowSize;
	int32 InventoryColSize;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> InventoryItems;

	bool bEquipInventory;
	FString EquipInventoryName;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> EquipInventoryItems;

	bool bEquipArmor;
	FString EquipArmorName;

	int32 StorageRowSize;
	int32 StorageColSize;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> StorageItems;

	int32 PlayerHealth;
	int32 PlayerArmor;

	UPROPERTY(BlueprintReadWrite)
	int32 money;

};
