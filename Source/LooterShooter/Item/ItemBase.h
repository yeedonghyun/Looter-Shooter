#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ItemData.h"

#include "ItemBase.generated.h"


UCLASS()
class LOOTERSHOOTER_API AItemBase : public AActor
{
    GENERATED_BODY()

public:
    AItemBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

protected:

    UStaticMeshComponent* StaticMeshComponent;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
        FItemData ItemData;

    UFUNCTION(BlueprintCallable)
        UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
};
