#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SoapBubbleBullet.generated.h"

UCLASS()
class LOOTERSHOOTER_API ASoapBubbleBullet : public AActor
{
	GENERATED_BODY()

public:
	ASoapBubbleBullet();

public:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadWrite)
	int damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionSphere;
};