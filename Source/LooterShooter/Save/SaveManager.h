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

	UPROPERTY(BlueprintReadWrite)
	int32 InventoryRowSize;
	UPROPERTY(BlueprintReadWrite)
	int32 InventoryColSize;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> InventoryItems;

	UPROPERTY(BlueprintReadWrite)
	bool bEquipInventory;
	UPROPERTY(BlueprintReadWrite)
	FString EquipInventoryName;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> EquipInventoryItems;

	UPROPERTY(BlueprintReadWrite)
	int32 StorageRowSize;
	UPROPERTY(BlueprintReadWrite)
	int32 StorageColSize;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotData> StorageItems;

	UPROPERTY(BlueprintReadWrite)
	bool bEquipWeapon;
	UPROPERTY(BlueprintReadWrite)
	FString EquipWeaponName;




	UPROPERTY(BlueprintReadWrite)
	int32 PlayerHealth;
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerArmor;
	UPROPERTY(BlueprintReadWrite)
	int32 money;


	//bool bEquipArmor;
	//FString EquipArmorName;
};
