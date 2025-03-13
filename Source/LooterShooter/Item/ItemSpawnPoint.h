#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class LOOTERSHOOTER_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawnPoint();

protected:
	virtual void BeginPlay() override;

protected:
    void SpawnItem();

public:
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TArray<TSubclassOf<class AItemBase>> ItemClasses;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    bool bAutoSpawnOnStart = true;
};
