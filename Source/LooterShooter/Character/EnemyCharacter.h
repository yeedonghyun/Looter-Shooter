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
	EEnemyState CurrentState;

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

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(int Damage);

	UPROPERTY(BlueprintReadWrite)
	int Health;

protected:

	UFUNCTION(BlueprintCallable)
	bool IsDetectPlayer();

	UFUNCTION(BlueprintCallable)
	bool IsAimedPlayer();

	UFUNCTION(BlueprintCallable)
	bool Fire();

	UFUNCTION(BlueprintCallable)
	void ResetShoot();

	UFUNCTION(BlueprintCallable)
	void ResetLastKnownPlayerLocation() { LastKnownPlayerLocation = FVector::ZeroVector; }

	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(FVector TargetLocation, float AcceptanceRadius);

	UFUNCTION(BlueprintCallable)
	bool MoveToTarget(AActor* TargetActor, float AcceptanceRadius);

	UFUNCTION(BlueprintCallable)
	void Rotate(float Degree, float RotationSpeed);

	UFUNCTION(BlueprintCallable)
	void RotateToTarget(AActor* TargetActor, float RotationSpeed);

	UFUNCTION(BlueprintCallable)
	FVector GetLastKnownPlayerLocation() { return LastKnownPlayerLocation; }

	UFUNCTION(BlueprintCallable)
	bool GetLostPlayer() { return LostPlayer; }

	UFUNCTION(BlueprintCallable)
	void SetLostPlayer(bool bLost) { LostPlayer = bLost; }

	UFUNCTION(BlueprintCallable)
	bool GetDetectePlayer() { return bDetectPlayer; }

	UFUNCTION(BlueprintCallable)
	bool GetShoot() { return bShoot; }

	UFUNCTION(BlueprintCallable)
	bool GetRecentDetectPlayer() { return bRecentDetectPlayer; }

	UFUNCTION(BlueprintCallable)
	AActor* GetWeapon() { return Weapon; }
	

	void CheckRecentlyDetectPlayer();

	bool bDetectPlayer;
	bool bShoot;
	bool LostPlayer;
	bool bRecentDetectPlayer;

	FTimerHandle ShootResetTimerHandle;
	FTimerHandle DetectPlayerTimerHandle;

	float MaxDetectionRange;
	float MaxDetectionAngle;
	float FireRate;
	float DetecteRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LastKnownPlayerLocation;

	TSubclassOf<AActor> BulletClass;
	APlayerCharacter* TargetPlayer;

	USkeletalMeshComponent* SkeletalMeshComponent;

	TSubclassOf<AWeapon> WeaponClass;
	AWeapon* Weapon;
};