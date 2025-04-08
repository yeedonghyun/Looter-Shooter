// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Item/ItemData.h"
//#include "../Widget/PlayerInventoryWidget.h"

#include "SaveManager.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API USaveManager : public USaveGame
{
	GENERATED_BODY()
	

public:
	//USaveManager();
	UFUNCTION(BlueprintCallable, Category = "Save")
		static void SaveSelectData(const TArray<FSlotData>& Items, const FString& SaveSlotName, const FString& DataName);
	UFUNCTION(BlueprintCallable, Category = "Save")
		static TArray<FSlotData> LoadSelectData(const FString& SaveSlotName, const FString& DataName);


	UFUNCTION(BlueprintCallable, Category = "Save")
		static void SaveData(const FString& SaveSlotName);
	UFUNCTION(BlueprintCallable, Category = "Save")
		static USaveManager* GetSaveInstance(const FString& SaveSlotName);
	UFUNCTION(BlueprintCallable, Category = "Save")
		static void SaveInventoryItems(const TArray<FSlotData>& Items, const FString& SaveSlotName);
	UFUNCTION(BlueprintCallable, Category = "Save")
		static TArray<FSlotData> LoadInventoryItems(const FString& SaveSlotName);



	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		TArray<FSlotData> InventoryItems;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		TArray<FSlotData> EquipInventoryItems;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		TArray<FSlotData> StorageItems;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 PlayerHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
		int32 PlayerArmor;
};
