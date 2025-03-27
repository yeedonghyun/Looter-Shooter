#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "../Bullet/Bullet.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class LOOTERSHOOTER_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	bool CanSeePlayer();
	void RotateToPlayer();
	void Fire();
	bool bShoot;
	void ResetShoot();
	void DetectePlayer();
	bool IsFacingPlayer();

	FTimerHandle ShootResetTimerHandle;
	FTimerHandle DetecteTimerHandle;

	float MaxDetectionRange;
	float MaxDetectionAngle;
	float RotationSpeed;
	float FireRate;
	float DetecteRate;
	float CachedDeltaTime;

	TSubclassOf<AActor> BulletClass;
	APlayerCharacter* TargetPlayer;
};
