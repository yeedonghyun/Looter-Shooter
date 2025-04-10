#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "../Bullet/Bullet.h"
#include "PlayerCharacter.h"
#include "../Gun/Weapon.h"
#include "EnemyCharacter.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle       UMETA(DisplayName = "Idle"),
	Patrol     UMETA(DisplayName = "Patrol"),
	Alert      UMETA(DisplayName = "Alert"),
	Combat     UMETA(DisplayName = "Combat")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EEnemyState CurrentState; // EnemyState를 UPROPERTY로 선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GunShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* CharacterReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GunReloadAnimation;

	int CurrentAmmo;

	UFUNCTION(BlueprintCallable)
	void UpdateWalkSpeed(float NewWalkSpeed);

protected:
	void IsSeePlayer();
	void RotateToPlayer(float DeltaTime);
	void Fire();
	void ResetShoot();
	void DetectePlayer();

	bool IsFacingPlayer();
	bool bSeePlayer;
	bool bShoot;

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

	USkeletalMeshComponent* SkeletalMeshComponent;

	TSubclassOf<AWeapon> WeaponClass;
	AWeapon* Weapon;
};