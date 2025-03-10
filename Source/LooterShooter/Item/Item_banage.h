#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Item_banage.generated.h"

UCLASS()
class LOOTERSHOOTER_API AItem_banage : public AItemBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
