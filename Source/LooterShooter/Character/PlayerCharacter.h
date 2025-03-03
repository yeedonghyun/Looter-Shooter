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
#include "PlayerCharacter.generated.h"

UCLASS()
class LOOTERSHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	enum PlayerState
	{
		IDEL,
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

	void CheckWallCloseInFront();

	UInputAction* MovementAction;
	UInputAction* CameraAction;
	UInputAction* RunAction;
	UInputAction* JumpAction;
	UInputAction* ReloadAction;
	UInputAction* CrouchAction;
	UInputAction* AimAction;
	UInputAction* ShootAction;
	
	APlayerCameraManager* Camera;
	APlayerController* PlayerController;
	USceneComponent* PivotComponent;

	UPlayerUIWidget* PlayerUI;

	FVector GunEndPoint;

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
	UAnimMontage* CharacterReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* GunReloadAnimation;

	int CurrentAmmo;
	int MagazineAmmo;
	bool bSemiFire;
	float Sensitivity;

private:
	//timeline
	
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

	FTimerHandle ShootResetTimerHandle;
	void ResetShoot();
	void ResetReload();
};
