#include "ItemSpawnPoint.h"
#include "ItemBase.h"
#include "Kismet/KismetMathLibrary.h"

AItemSpawnPoint::AItemSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

    if (bAutoSpawnOnStart)
    {
        SpawnItem();
    }
}
void AItemSpawnPoint::SpawnItem()
{
    if (ItemClasses.Num() > 0)
    {
        int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, ItemClasses.Num() - 1);
        TSubclassOf<AItemBase> SelectedItemClass = ItemClasses[RandomIndex];

        if (SelectedItemClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            GetWorld()->SpawnActor<AItemBase>(SelectedItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
        }
    }
}