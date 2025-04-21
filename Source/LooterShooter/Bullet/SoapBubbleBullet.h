#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoapBubbleBullet.generated.h"

UCLASS()
class LOOTERSHOOTER_API ASoapBubbleBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoapBubbleBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
