// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Character/EnemyCharacter.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class LOOTERSHOOTER_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnPoint();

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float SpawnProbability = 1.f;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class AEnemyCharacter> EnemyClass;

    AEnemyCharacter* SpawnEnemy();
};
