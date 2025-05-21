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

	//money = 0;
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


		//NewSave->InventoryRowSize = 2;
		//NewSave->InventoryColSize = 5;

		//NewSave->bEquipInventory = false;
		//NewSave->EquipInventoryName = "bag";

		//NewSave->bEquipArmor = false;
		//NewSave->EquipArmorName = "";

		//NewSave->StorageRowSize = 15;
		//NewSave->StorageColSize = 7;

		//NewSave->PlayerHealth = 10;
		//NewSave->PlayerArmor = 0;

		NewSave->money = 0;




		UGameplayStatics::SaveGameToSlot(NewSave, SaveSlotName, 0);
		return NewSave;
	}
}

void USaveManager::SaveDataSet(const FString& SaveSlotName, USaveManager* SaveGameInstance)
{
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
}

void CreateNewSaveData(const FString& SaveSlotName)
{
	USaveManager* NewSave = Cast<USaveManager>(UGameplayStatics::CreateSaveGameObject(USaveManager::StaticClass()));

	UGameplayStatics::SaveGameToSlot(NewSave, SaveSlotName, 0);
}