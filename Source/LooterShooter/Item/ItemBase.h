#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    WEAPON UMETA(DisplayName = "Weapon"),
    AMMO UMETA(DisplayName = "Ammo"),
    HEALING UMETA(DisplayName = "Healing")
};

USTRUCT(BlueprintType)
struct FInventoryLocation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Y;

    FInventoryLocation() : X(-1), Y(-1) {}
    FInventoryLocation(int32 InX, int32 InY) : X(InX), Y(InY) {}
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

    UStaticMeshComponent* StaticMeshComponent;
    FInventoryLocation Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    UImage* Image;

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
    FInventoryLocation GetInventoryLocation() const { return Location; }

    UFUNCTION(BlueprintCallable)
    EItemType GetItemType() const { return Type; }

    UFUNCTION(BlueprintCallable)
    UImage* GetImage() const { return Image; }
};
