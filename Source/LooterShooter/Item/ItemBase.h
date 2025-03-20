#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    WEAPON UMETA(DisplayName = "Weapon"),
    AMMO UMETA(DisplayName = "Ammo"),
    HEALING UMETA(DisplayName = "Healing"),
    BAG UMETA(DisplayName = "Bag")
};

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    int32 Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    int32 Weight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    EItemType Type;

    UStaticMeshComponent* StaticMeshComponent;

public:
    UFUNCTION(BlueprintCallable)
    FString GetName() const { return Name; }

    UFUNCTION(BlueprintCallable)
    int32 GetValue() const { return Value; }

    UFUNCTION(BlueprintCallable)
    int32 GetWeight() const { return Weight; }

    UFUNCTION(BlueprintCallable)
    UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

    UFUNCTION(BlueprintCallable)
    EItemType GetItemType() const { return Type; }
};
