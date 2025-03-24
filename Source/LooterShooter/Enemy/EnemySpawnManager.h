#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.h"
#include "EnemySpawnManager.generated.h"

UCLASS()
class LOOTERSHOOTER_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnManager();

	virtual void BeginPlay() override;

protected:
    void FindSpawnPoints();
    void SpawnEnemies();

    TArray<AEnemySpawnPoint*> SpawnPoints;
};
