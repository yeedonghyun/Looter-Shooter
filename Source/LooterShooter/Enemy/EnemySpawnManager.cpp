#include "EnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"

AEnemySpawnManager::AEnemySpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnManager::BeginPlay()
{
    Super::BeginPlay();

    FindSpawnPoints();
    SpawnEnemies();
}

void AEnemySpawnManager::FindSpawnPoints()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnPoint::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (AEnemySpawnPoint* SpawnPoint = Cast<AEnemySpawnPoint>(Actor))
        {
            SpawnPoints.Add(SpawnPoint);
        }
    }
}

void AEnemySpawnManager::SpawnEnemies()
{
    for (AEnemySpawnPoint* SpawnPoint : SpawnPoints)
    {
        if (SpawnPoint)
        {
            SpawnPoint->SpawnEnemy();
        }
    }
}
