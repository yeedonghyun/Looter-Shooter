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
}

#if WITH_EDITOR
void AItem_Inventory::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    int32 TotalSlots = Height * Width;
    savedItems.SetNum(TotalSlots);
}
#endif