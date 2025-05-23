// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Inventory.h"

void AItem_Inventory::BeginPlay()
{
    Super::BeginPlay();

    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("StaticlMeshComponent")))
        {
            StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
            break;
        }
    }

    if (InventoryType == EInventoryType::BOX)
    {
        TArray<FString> itemList = { "Ammo1" , "Armor1" ,"Armor3" ,"bag48" ,"Syringe" ,"Medikit" };
        int randCnt = FMath::RandRange(0, 4);
        int cnt = 0;

        for (int i = 0; i < randCnt; i++)
        {
            int randItemIdx = FMath::RandRange(0, itemList.Num() - 1);

            while (1)
            {
                int randIdx = FMath::RandRange(0, savedItems.Num() - 1);

                if (savedItems[randIdx].bHaveItem)
                {
                    continue;
                }

                FString name = itemList[randItemIdx];
                FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *name, *name);

                if (TSubclassOf<AItemBase> ItemClass = LoadClass<AItemBase>(nullptr, *FullPath))
                {
                    AItemBase* DefaultObject = ItemClass->GetDefaultObject<AItemBase>();

                    if (DefaultObject)
                    {
                        savedItems[randIdx].SetSlotFromItemData(DefaultObject->ItemData);
                        break;
                    }
                }
            }
        }

    }
}

#if WITH_EDITOR
void AItem_Inventory::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    int32 TotalSlots = Height * Width;
    savedItems.SetNum(TotalSlots);
}
#endif