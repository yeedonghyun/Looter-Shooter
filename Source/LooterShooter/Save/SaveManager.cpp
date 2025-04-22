// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"

USaveManager::USaveManager()
{
	InventoryRowSize = 2;
	InventoryColSize = 5;

	bEquipInventory = false;
	EquipInventoryName = "bag";

	bEquipArmor = false;
	EquipArmorName = "";

	StorageRowSize = 15;
	StorageColSize = 7;

	PlayerHealth = 10;
	PlayerArmor = 0;
}

USaveManager* USaveManager::GetSaveInstance(const FString& SaveSlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		return Cast<USaveManager>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}

	else
	{
		USaveManager* NewSave = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));
		UGameplayStatics::SaveGameToSlot(NewSave, SaveSlotName, 0);
		return NewSave;
	}
}

void USaveManager::SaveDataSet(const FString& SaveSlotName, USaveManager* SaveGameInstance)
{

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);

	//USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	//if (SaveGameInstance)
	//{
	//	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	//}
}


void USaveManager::SaveData(const FString& SaveSlotName)
{
	USaveManager* SaveGameInstance = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	if (SaveGameInstance)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	}
}
