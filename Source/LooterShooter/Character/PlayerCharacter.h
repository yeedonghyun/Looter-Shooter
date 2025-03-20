// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Widget/PlayerUIWidget.h"
#include "../Gun/Weapon.h"
#include "../Item/Item_banage.h"
#include "../Widget/PlayerInventoryWidget.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LOOTERSHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	enum PlayerState
	{
		IDLE,
		RUN,
		MOVEMENT,
		JUMP,
		RELOAD
	}; 

	PlayerState curState;

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
	void Reload(const FInputActionValue& InputValue);

	void Run(const FInputActionValue& InputValue);
	void UnRun(const FInputActionValue& InputValue);

	void Crouch(const FInputActionValue& InputValue);
	void UnCrouch(const FInputActionValue& InputValue);

	void Aim(const FInputActionValue& InputValue);
	void UnAim(const FInputActionValue& InputValue);

	void Shoot(const FInputActionValue& InputValue);
	void UnShoot(const FInputActionValue& InputValue);

	void PickUpItem(const FInputActionValue& InputValue);
	void CreateItem(const FInputActionValue& InputValue);

	void CheckObjectCloseAhead();
	void CheckWall(FVector Start, FRotator Rotation, int ViewDis);
	void CheckItem(FVector Start, FRotator Rotation, int ViewDis);
	void ToggleInventory(const FInputActionValue& InputValue);

	UInputAction* MovementAction;
	UInputAction* CameraAction;
	UInputAction* RunAction;
	UInputAction* JumpAction;
	UInputAction* ReloadAction;
	UInputAction* CrouchAction;
	UInputAction* AimAction;
	UInputAction* ShootAction;
	UInputAction* PickUpItemAction;
	UInputAction* CreateItemAction;
	UInputAction* InventoryAction;

	APlayerCameraManager* Camera;
	APlayerController* PlayerController;
	USceneComponent* PivotComponent;

	UPlayerUIWidget* PlayerUI;
	FVector GunEndPoint;

	UPlayerInventoryWidget* InventoryUI;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bWallCloseInFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bReload;

	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ShootAimedAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GunShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* CharacterReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GunReloadAnimation;

	int CurrentAmmo;
	int MagazineAmmo;
	bool bSemiFire;
	float Sensitivity;

private:
	//Run
	UPROPERTY(EditAnywhere)
	UCurveFloat* RunCurve;

	FOnTimelineFloat RunTimeLineUpdateDelegate;
	FOnTimelineEvent RunTimeLineFinishDelegate;

	FTimeline RunTimeline;

	UFUNCTION()
	void RunStart(float Output);

	UFUNCTION()
	void RunEnd();

	//Crouch
	UPROPERTY(EditAnywhere)
	UCurveFloat* CrouchCurve;

	FOnTimelineFloat CrouchTimeLineUpdateDelegate;
	FOnTimelineEvent CrouchTimeLineFinishDelegate;

	FTimeline CrouchTimeline;

	UFUNCTION()
	void CrouchStart(float Output);

	UFUNCTION()
	void CrouchEnd();

	//shoot
	FTimerHandle ShootResetTimerHandle;

	void ResetShoot();
	void ResetReload();

	//item
	TSubclassOf<AWeapon> WeaponClass;
	AWeapon* Weapon;
	AItemBase* AimedItem;

	//stamina
	float curStamina;
	float curHandStamina;

	bool Tired;
	bool HandTired;

	float timerRepeatTime;

	FTimerHandle HandStaminaTimerHandle;
	FTimerHandle StaminaTimerHandle;

	UFUNCTION()
	void StaminaControl();

	UFUNCTION()
	void HandStaminaControl();
};
