
#include "EnemySpawnPoint.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;
}

AEnemyCharacter* AEnemySpawnPoint::SpawnEnemy()
{
    if (FMath::FRand() <= SpawnProbability && EnemyClass)
    {
        return GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, GetActorLocation(), GetActorRotation());
    }
    return nullptr;
}