// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"




void USaveManager::SaveData(const FString& SaveSlotName)
{
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	if (SaveGameInstance)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	}
}

USaveManager* USaveManager::GetSaveInstance(const FString& SaveSlotName)
{
	USaveManager* LoadGameInstance = Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (LoadGameInstance == nullptr)
	{
		LoadGameInstance = GetMutableDefault<USaveManager>();
		SaveData(SaveSlotName);
	}

	return LoadGameInstance;
}


void USaveManager::SaveInventoryItems(const TArray<FSlotData>& Items, const FString& SaveSlotName)
{
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->InventoryItems = Items;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	}
}

TArray<FSlotData> USaveManager::LoadInventoryItems(const FString& SaveSlotName)
{
	USaveManager* SaveInstance = GetSaveInstance(SaveSlotName);
	return SaveInstance->InventoryItems;
}



void USaveManager::SaveSelectData(const TArray<FSlotData>& Items, const FString& SaveSlotName, const FString& DataName)
{
	USaveManager* SaveInstance = GetSaveInstance(SaveSlotName);

	if (SaveInstance)
	{
		if (DataName == "Inventory")
		{
			SaveInstance->InventoryItems = Items;
		}
		else if (DataName == "Storage")
		{
			SaveInstance->StorageItems = Items;
		}

		else if (DataName == "PlayerBag")
		{
			SaveInstance->EquipInventoryItems = Items;
		}

		UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);
	}


	//USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	//if (SaveGameInstance)
	//{
	//	if (DataName == "Inventory")
	//	{
	//		SaveGameInstance->InventoryItems = Items; 
	//	}
	//	else if (DataName == "Storage")
	//	{
	//		SaveGameInstance->StorageItems = Items;  
	//	}

	//	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	//}
}

TArray<FSlotData> USaveManager::LoadSelectData(const FString& SaveSlotName, const FString& DataName)
{
	USaveManager* SaveInstance = GetSaveInstance(SaveSlotName);

	if (SaveInstance)
	{
		if (DataName == "Inventory")
		{
			return SaveInstance->InventoryItems;
		}
		else if (DataName == "Storage")
		{
			return SaveInstance->StorageItems;
		}

		else if (DataName == "PlayerBag")
		{
			return SaveInstance->EquipInventoryItems;
		}
	}

	return TArray<FSlotData>();
}


