#include "PlayerCharacter.h"
#include "../Bullet/Bullet.h"
#include "../Bullet/SoapBubbleBullet.h"
#include "../Character/EnemyCharacter.h"
#include <Kismet/GameplayStatics.h>

APlayerCharacter::APlayerCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    curState = PlayerState::IDLE;
    bCrouch = false;
    bAiming = false;
    bWallCloseInFront = false;
    bRun = false;
    bSemiFire = false;
    Sensitivity = 0.4;
    CurrentAmmo = 30;
    MagazineAmmo = 30;

    MaxAmmo = 0;

    AimedItem = nullptr;
    curHandStamina = 1.f;
    curStamina = 1.f;
    timerRepeatTime = 0.01;
    Tired = false;
    HandTired = false;

    bOpenInventory = false;
    ElapsedTime = 0.0f;
    bIsTimerActive = true;

    bUsingItem = false;
    ItemUseDuration = 0.0f;
    ItemUseDelay = 1.0f;

    bIsEscaping = false;
    EscapeDuration = 0.0f;
    EscapeDelay = 1.0f;
    bIskeyTutorialActive = false;

    EscapeTime = 5.f;

	GunEndPoint = FVector::ZeroVector;
    TargetOffset = FVector::ZeroVector;

    IMC = LoadObject<UInputMappingContext>(nullptr, 
        TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Data/IMC_FPS.IMC_FPS'")); 
    
    MovementAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Movement.IA_Movement'"));

    CameraAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Camera.IA_Camera'"));

    RunAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Run.IA_Run'"));

    JumpAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Jump.IA_Jump'"));

    ReloadAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Reload.IA_Reload'"));

    CrouchAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Crouch.IA_Crouch'"));

    AimAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Aim.IA_Aim'"));

    ShootAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Shoot.IA_Shoot'"));

    PickUpItemAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_PickUpItem.IA_PickUpItem'"));

    CreateItemAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_CreateItem.IA_CreateItem'"));

    RunCurve = LoadObject<UCurveFloat>(nullptr,
        TEXT("/Script/Engine.CurveFloat'/Game/Data/TimeLineCourve/RunFOV.RunFOV'"));
    
    CrouchCurve = LoadObject<UCurveFloat>(nullptr,
        TEXT("/Script/Engine.CurveFloat'/Game/Data/TimeLineCourve/CrouchCapsuleHalf.CrouchCapsuleHalf'"));

    InventoryAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Inventory.IA_Inventory'"));

	leftTiltAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_leftTilt.IA_leftTilt'"));

    RightAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_RightTilt.IA_RightTilt'"));

    KeyTutorialAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_KeyTutorial.IA_KeyTutorial'"));

    BulletClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_Bullet.BP_Bullet_C'"));
	SoupBubbleClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_SoapBubbleBullet.BP_SoapBubbleBullet_C'"));

    static ConstructorHelpers::FClassFinder<AActor> WeaponBP(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Gun/BP_Weapon1.BP_Weapon1_C'"));
    if (WeaponBP.Succeeded())
    {
        WeaponClass = WeaponBP.Class;
    }
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    PlayerController = GetWorld()->GetFirstPlayerController();
    Camera = PlayerController->PlayerCameraManager;

    USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");
    Health = SaveData->PlayerHealth;
    Armor = SaveData->PlayerArmor;



    for (UActorComponent* Child : GetComponents())
    {
        if (USceneComponent* SceneChild = Cast<USceneComponent>(Child))
        {
            if (SceneChild->GetName() == TEXT("Pivot"))
            {
                PivotComponent = SceneChild;
                break;
            }
        }
    }

    if (PlayerController)
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                InputSubsystem->AddMappingContext(IMC, 0);
            }
        }
    }

    if (RunCurve)
    {
        RunTimeLineUpdateDelegate.BindUFunction(this, FName("RunStart"));
        RunTimeLineFinishDelegate.BindUFunction(this, FName("RunEnd"));

        RunTimeline.AddInterpFloat(RunCurve, RunTimeLineUpdateDelegate);
        RunTimeline.SetTimelineFinishedFunc(RunTimeLineFinishDelegate);
    }

    if (CrouchCurve) 
    {
        CrouchTimeLineUpdateDelegate.BindUFunction(this, FName("CrouchStart"));
        CrouchTimeLineFinishDelegate.BindUFunction(this, FName("CrouchEnd"));

        CrouchTimeline.AddInterpFloat(CrouchCurve, CrouchTimeLineUpdateDelegate);
        CrouchTimeline.SetTimelineFinishedFunc(CrouchTimeLineFinishDelegate);
    }


    if (TSubclassOf<UUserWidget> PlayerUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_PlayerUIWidget.BP_PlayerUIWidget_C'")))
    {
        PlayerUI = CreateWidget<UPlayerUIWidget>(GetWorld(), PlayerUIClass);
        if (PlayerUI)
        {
            PlayerUI->AddToViewport();

            PlayerUI->SetHandStamina(curHandStamina);
            PlayerUI->SetStamina(curStamina);

            PlayerUI->SetHealth(Health / MaxHealth);
            PlayerUI->SetArmor(Armor / MaxArmor);

            PlayerUI->SetLeftAmmoText(CurrentAmmo);
            PlayerUI->SetMagazineText(MaxAmmo);
        }
    }

    if (TSubclassOf<UUserWidget> FadeInAndOutClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_FadeInAndOutWidget.BP_FadeInAndOutWidget_C'")))
    {
        FadeInAndOut = CreateWidget<UFadeInAndOutWidget>(GetWorld(), FadeInAndOutClass);
		if (FadeInAndOut)
		{
			FadeInAndOut->AddToViewport();
			FadeInAndOut->PlayFadeOut();
		}
    }

    if (TSubclassOf<UUserWidget> HitAndHealIndicatorUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_HitAndHealIndicatorWidget.BP_HitAndHealIndicatorWidget_C'")))
    {
        HitAndHealIndicatorUI = CreateWidget<UHitAndHealIndicatorWidget>(GetWorld(), HitAndHealIndicatorUIClass);
        if (HitAndHealIndicatorUI)
        {
            HitAndHealIndicatorUI->AddToViewport();
        }
    }

    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("SkeletalMeshComponent")))
        {
            SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component);
            BaseRelLocation = SkeletalMeshComponent->GetRelativeLocation();
            BaseRelRotation = SkeletalMeshComponent->GetRelativeRotation();
            break;
        }
    }

    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;

        AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(
           WeaponClass,
            Location,
            Rotation,
            SpawnParams
        );

        if (SpawnedWeapon)
        {
            FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);

            SpawnedWeapon->AttachToComponent(
                SkeletalMeshComponent,
                AttachRules,
                FName("ik_hand_gun")
            );

            Weapon = Cast<AWeapon>(SpawnedWeapon);
        }
    }

    LoadInventoryClass();
    InventoryUI->SetHealth(Health / MaxHealth);
    InventoryUI->SetArmor(Armor / MaxArmor);

    if (TSubclassOf<UUserWidget> KeyTutorialUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_KeyTutorialWidget.BP_KeyTutorialWidget_C'")))
    {
        KeyTutorialUI = CreateWidget<UKeyTutorialWidget>(GetWorld(), KeyTutorialUIClass);
        if (KeyTutorialUI)
        {
            KeyTutorialUI->AddToViewport();
        }
    }


    GetWorldTimerManager().SetTimer(HandStaminaTimerHandle, this, &APlayerCharacter::HandStaminaControl, timerRepeatTime, true);
    GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::StaminaControl, timerRepeatTime, true);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RunTimeline.TickTimeline(DeltaTime);
    CrouchTimeline.TickTimeline(DeltaTime);

    auto CamearaRotation = PlayerController->GetControlRotation();
    PivotComponent->SetRelativeRotation(FRotator(CamearaRotation.Pitch, 0, 0));

    CheckObjectCloseAhead();

    if (bIsTimerActive)
    {
        ElapsedTime += DeltaTime;
        PlayerUI->UpdateTimerUI(ElapsedTime);
    }

    if (bUsingItem)
    {
        UpdateItemUseDuration(DeltaTime);
    }

    if (PlayerController)
    {
        FVector2D NewOffset = FMath::Vector2DInterpTo(
            TargetRecoilOffset,
            FVector2D::ZeroVector,
            DeltaTime,
            RecoilRecoverySpeed
        );

        FVector2D DeltaOffset = NewOffset - CurrentRecoilOffset;

        PlayerController->AddPitchInput(-DeltaOffset.X);
        PlayerController->AddYawInput(DeltaOffset.Y);

        CurrentRecoilOffset = NewOffset;
        TargetRecoilOffset = NewOffset;
    }

    if (bIsEscaping)
    {
        UpdateEscapeDuration(DeltaTime);
    }

    if (bRun)
    {
        FVector Velocity = GetVelocity();
        FVector Forward = GetActorForwardVector();
        FVector MovementDirection = Velocity.GetSafeNormal2D();

        float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, MovementDirection)));

        if (Angle > 50.0f)
        {
            UnRun(FInputActionValue());
        }
    }

    FRotator CurrRelRot = SkeletalMeshComponent->GetRelativeRotation();
    CurrRelRot.Pitch = FMath::FInterpTo(
        CurrRelRot.Pitch,
        BaseRelRotation.Pitch + TargetRoll,
        DeltaTime,
        TiltInterpSpeed
    );
    SkeletalMeshComponent->SetRelativeRotation(CurrRelRot);

    FVector CurrRelLoc = SkeletalMeshComponent->GetRelativeLocation();
    FVector DesiredRelLoc = BaseRelLocation + TargetOffset;
    FVector NewRelLoc = FMath::VInterpTo(
        CurrRelLoc,
        DesiredRelLoc,
        DeltaTime,
        TiltInterpSpeed
    );
    SkeletalMeshComponent->SetRelativeLocation(NewRelLoc);
}

void APlayerCharacter::CheckObjectCloseAhead()
{
    FVector Start;
    FRotator Rotation;

    PlayerController->GetPlayerViewPoint(Start, Rotation);

    CheckWall(Start, Rotation, 70);
    CheckItem(Start, Rotation, 300);
}

void APlayerCharacter::CheckWall(FVector Start, FRotator Rotation, int ViewDis)
{
    FHitResult HitOut;

    GunEndPoint = ((Rotation.Vector() * ViewDis) + Start);
    FCollisionQueryParams _traceParams;

    bWallCloseInFront = GetWorld()->LineTraceSingleByChannel(HitOut, Start, GunEndPoint, ECC_Visibility, _traceParams);
}

void APlayerCharacter::CheckItem(FVector Start, FRotator Rotation, int ViewDis)
{
    FHitResult HitOut;
    FVector EndPoint = Start + Rotation.Vector() * ViewDis;

    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(this);

    bool bCollision = GetWorld()->LineTraceSingleByChannel(HitOut, Start, EndPoint, ECC_WorldStatic, TraceParams);
    AActor* HitActor = HitOut.GetActor();
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitActor);
    AWeapon* DropWeapon = Cast<AWeapon>(HitActor);

    const bool bIsItem = HitActor && HitActor->IsA<AItemBase>();
    const bool bIsDeadEnemy = Enemy && Enemy->GetCurrentState() == EEnemyState::Dead;
    const bool bIsWeaponDroped = DropWeapon && DropWeapon->bIsDrroped;

    if (bCollision && (bIsItem || bIsDeadEnemy || bIsWeaponDroped))
    {
		if (bIsItem)
		{
            AimedItem = Cast<AItemBase>(HitActor);
            bIsAimedDropedWeapon = false;
		}
		else if (bIsDeadEnemy)
		{
            AimedItem = Enemy->GetInventory();
		}
		else if (bIsWeaponDroped)
		{
            AimedItem = DropWeapon->GunItem;
            DropedWeapon = DropWeapon;
            bIsAimedDropedWeapon = true;
		}

        if (PlayerUI)
        {
            PlayerUI->ShowCrosshairOnAimEnd();
            PlayerUI->ToggleInfoUI(true);
            PlayerUI->UpdateInfoUI(AimedItem->ItemData, true);

            if (AItem_Inventory* Inventory = Cast<AItem_Inventory>(AimedItem))
            {
                if (Inventory->InventoryType == EInventoryType::BOX)
                {
                    PlayerUI->UpdateInfoUI(AimedItem->ItemData, false);
                }
            }
        }
    }
    else
    {
        AimedItem = nullptr;

        if (PlayerUI)
        {
            PlayerUI->HideCrosshairOnAim();
            PlayerUI->ToggleInfoUI(false);
        }

        if (InventoryUI && InventoryUI->bWorldInventoryOpen)
        {
            InventoryUI->DeleteWorldInventory();
        }
    }
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        FEnhancedInputActionHandlerValueSignature::TMethodPtr<APlayerCharacter> MethodPointer = &APlayerCharacter::Move;
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnMove;
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Look;
        EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Jump;
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Reload;
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Run;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, MethodPointer);
        
        MethodPointer = &APlayerCharacter::UnRun;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Crouch;
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnCrouch;
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Aim;
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnAim;
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Shoot;
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnShoot;
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::PickUpItem;
        EnhancedInputComponent->BindAction(PickUpItemAction, ETriggerEvent::Started, this, MethodPointer);

        MethodPointer = &APlayerCharacter::CreateItem;
        EnhancedInputComponent->BindAction(CreateItemAction, ETriggerEvent::Started, this, MethodPointer);

        MethodPointer = &APlayerCharacter::ToggleInventory;
        EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::LeftTilt;
        EnhancedInputComponent->BindAction(leftTiltAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnLeftTilt;
        EnhancedInputComponent->BindAction(leftTiltAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::RightTilt;
        EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnRightTilt;
        EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::ToggleKeyTutorial;
        EnhancedInputComponent->BindAction(KeyTutorialAction, ETriggerEvent::Started, this, MethodPointer);
    }
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    if (bOpenInventory || curState == PlayerState::DEAD || bIskeyTutorialActive)
        return;
    
    curState = PlayerState::MOVEMENT;

    FVector2D MovementVector = InputValue.Get<FVector2D>();
    const FVector Forward = GetActorForwardVector();
    const FVector Right = GetActorRightVector();

    const FVector Direction = (Forward * MovementVector.Y) + (Right * MovementVector.X);

    AddMovementInput(Direction, 1.0f);
    
}

void APlayerCharacter::UnMove(const FInputActionValue& InputValue)
{
    if (curState == PlayerState::DEAD)
        return;

    if (bRun)
    {
        UnRun(InputValue);
    }
    curState = PlayerState::IDLE;
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
    if (bOpenInventory || curState == PlayerState::DEAD || bIskeyTutorialActive)
        return;
    

    FVector2D LookVector = InputValue.Get<FVector2D>();

    AddControllerYawInput(LookVector.X * Sensitivity);
    AddControllerPitchInput(-LookVector.Y * Sensitivity);
}

void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    if (curStamina < 0.1f || GetCharacterMovement()->IsFalling() || bOpenInventory || curState == PlayerState::DEAD || bUsingItem || bIskeyTutorialActive) {
        return;
    }

    Jump();
}

void APlayerCharacter::Reload(const FInputActionValue& InputValue)
{
    if (bReload || bShoot || bOpenInventory || curState == PlayerState::DEAD || MaxAmmo <= 0 || bUsingItem || bIskeyTutorialActive) {
        return;
    }

    if (bRun) {
        UnRun(InputValue);
    }

    curState = PlayerState::RELOAD;
    bReload = true;

    SkeletalMeshComponent->GetAnimInstance()->Montage_Play(CharacterReloadAnimation, 1.f);
    Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Play(GunReloadAnimation, 1.f);

    float AnimationDuration = 1.9;
    GetWorldTimerManager().SetTimer(ShootResetTimerHandle, this, &APlayerCharacter::ResetReload, AnimationDuration, false);
}

void APlayerCharacter::ResetReload()
{
    if (curState == PlayerState::DEAD)
        return;
    
    if (InventoryUI)
    {
        CurrentAmmo = InventoryUI->GetAmmo(MagazineAmmo - CurrentAmmo) + CurrentAmmo;
        MaxAmmo = InventoryUI->sumAmmo;
    }


    if (PlayerUI) {

        PlayerUI->SetLeftAmmoText(CurrentAmmo);
        PlayerUI->SetMagazineText(MaxAmmo);
    }

    bReload = false;
}

void APlayerCharacter::Run(const FInputActionValue& InputValue)
{
    if (Tired || bReload || bAiming || bCrouch || bOpenInventory || curState == PlayerState::IDLE || curState == PlayerState::DEAD || bUsingItem)
    {
        return;
    }

    FVector Velocity = GetVelocity();
    float Speed2D = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

    FVector Forward = GetActorForwardVector();
    FVector MovementDirection = Velocity.GetSafeNormal2D(); 
    float DotProduct = FVector::DotProduct(Forward, MovementDirection);

    if (Speed2D > 0.1f && DotProduct > 0.5f)
    {
        curState = PlayerState::RUN;
        bRun = true;

        RunTimeline.Play();
        GetCharacterMovement()->MaxWalkSpeed = 600;
    }
}

void APlayerCharacter::UnRun(const FInputActionValue& InputValue)
{
    RunTimeline.Reverse();
    GetCharacterMovement()->MaxWalkSpeed = 300;

    FVector Velocity = GetVelocity();
    float Speed2D = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
    bRun = false;

    if (Speed2D <= 0.1) 
    {
        curState = PlayerState::MOVEMENT;
    }
    else
    {
        curState = PlayerState::IDLE;
    }
}

void APlayerCharacter::ApplyDamage(int Damage)
{
	if (curState == PlayerState::DEAD) return;

    if (Armor > 0)
    {
        Armor -= Damage;
        
        if (Armor <= 0.f) {
            Armor = 0.f;
        }
        PlayerUI->SetArmor(Armor / MaxArmor);
    }
    else
    {
        Health -= Damage;
        PlayerUI->SetHealth(Health / MaxHealth);

        if (Health <= 0.f)
        {
            curState = PlayerState::DEAD;

            USaveManager* SaveData = USaveManager::GetSaveInstance("Save1");
            SaveData->PlayerHealth = 100;
            SaveData->bEquipWeapon = false;

            for (int32 i = 0; i < SaveData->InventoryItems.Num(); i++)
            {
                if (SaveData->InventoryItems[i].bHaveItem)
                {
                    SaveData->InventoryItems[i].bHaveItem = false;
                }
            }

            SaveData->bEquipInventory = false;

            for (int32 i = 0; i < SaveData->EquipInventoryItems.Num(); i++)
            {
                if (SaveData->EquipInventoryItems[i].bHaveItem)
                {
                    SaveData->EquipInventoryItems[i].bHaveItem = false;
                }
            }

            USaveManager::SaveDataSet("Save1", SaveData);



            ReturnToMain();



            if (SkeletalMeshComponent)
            {
                SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
                SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
                SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

                SkeletalMeshComponent->bPauseAnims = true;
                SkeletalMeshComponent->bBlendPhysics = true;

                SkeletalMeshComponent->SetAllBodiesSimulatePhysics(true);
                SkeletalMeshComponent->WakeAllRigidBodies();
            }

            return;
        }
    }

	if (HitAndHealIndicatorUI)
	{
		HitAndHealIndicatorUI->PlayHit();
	}
}

void APlayerCharacter::RunStart(float Output)
{
    if (curState == PlayerState::DEAD)
        return;

    if (Camera)
    {
        Camera->SetFOV(Output);
    }
}

void APlayerCharacter::RunEnd()
{
    return;
}

void APlayerCharacter::Crouch(const FInputActionValue& InputValue)
{
    if (curState == PlayerState::DEAD)
		return;

    if (bRun) {
        UnRun(InputValue);
    }

    Crouch();
}

void APlayerCharacter::UnCrouch(const FInputActionValue& InputValue)
{
    UnCrouch();
}

void APlayerCharacter::Aim(const FInputActionValue& InputValue)
{
    if (HandTired || bOpenInventory || curState == PlayerState::DEAD) {
        return;
    }

    if (curState == PlayerState::RUN)
        UnRun(InputValue);

    bAiming = true;
}

void APlayerCharacter::UnAim(const FInputActionValue& InputValue)
{
    bAiming = false;
}

void APlayerCharacter::Shoot(const FInputActionValue& InputValue)
{
    if (CurrentAmmo <= 0 || bShoot || bRun || bReload || bOpenInventory || curState == PlayerState::DEAD || bIskeyTutorialActive)
        return;

    if (PlayerUI) {
        CurrentAmmo--;
        PlayerUI->SetLeftAmmoText(CurrentAmmo);
    }

    SkeletalMeshComponent->GetAnimInstance()->Montage_Play(ShootAnimation, 1.f);
    Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Play(GunShootAnimation, 1.f);
    
    //// 일반 총알
    //if (BulletClass && Weapon) {
    //    FVector MuzzleLocation = Weapon->GetEndPointLocation();
    //    FRotator FireRotation = Camera->GetCameraRotation();

    //    if (!bAiming) {
    //        float RandomYaw = FMath::RandRange(-2.0f, 2.0f);
    //        float RandomPitch = FMath::RandRange(-1.0f, 1.0f);
    //        FireRotation.Yaw += RandomYaw;
    //        FireRotation.Pitch += RandomPitch;
    //    }

    //    GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, FireRotation);
    //    Weapon->SpawnMuzzleFlash();
    //}

    //비눗방울
    if (SoupBubbleClass && Weapon) {
        GetWorld()->SpawnActor<ASoapBubbleBullet>(SoupBubbleClass, Weapon->GetEndPointLocation(), Camera->GetCameraRotation());
        Weapon->SpawnMuzzleFlash();
    }
    
    AddRecoil();

    bShoot = true;

    float AnimationDuration = 0.2;
    GetWorldTimerManager().SetTimer(ShootResetTimerHandle, this, &APlayerCharacter::ResetShoot, AnimationDuration, false);
}

void APlayerCharacter::AddRecoil()
{
    if (!PlayerController || curState == PlayerState::DEAD) 
        return;

    float RecoilPitch = FMath::RandRange(0.5f, 1.0f);
    float RecoilYaw = FMath::RandRange(-1.0f, 1.0f);

    if (!bAiming) {
        TargetRecoilOffset += FVector2D(RecoilPitch * 0.1f, RecoilYaw * 0.1f);
    }
    else {
        TargetRecoilOffset += FVector2D(RecoilPitch, RecoilYaw);
    }
}

void APlayerCharacter::DestroyItemPerAimedItemType()
{
    if (bIsAimedDropedWeapon) {
		if (DropedWeapon)
		{
			DropedWeapon->Destroy();
			DropedWeapon = nullptr;
		}
    }
    else {
        AimedItem->Destroy();
    }
}

void APlayerCharacter::Jump()
{
    curStamina = FMath::Max(curStamina - 0.2f, 0.0f);
    PlayerUI->SetStamina(curStamina);

    Super::Jump();
    curState = PlayerState::JUMP;
}

void APlayerCharacter::Crouch(bool bClientSimulation)
{
    CrouchTimeline.Play();
    RunTimeline.Reverse();
    GetCharacterMovement()->MaxWalkSpeed = 200;
    bCrouch = true;
}

void APlayerCharacter::UnCrouch(bool bClientSimulation)
{
    CrouchTimeline.Reverse();
    GetCharacterMovement()->MaxWalkSpeed = 300;
    curState = PlayerState::IDLE;
    bCrouch = false;
}

void APlayerCharacter::ResetShoot()
{
    bShoot = false;
}

void APlayerCharacter::UnShoot(const FInputActionValue& InputValue)
{
    bShoot = false;
}

void APlayerCharacter::PickUpItem(const FInputActionValue& InputValue)
{
    if (AimedItem) {

        if (AimedItem->ItemData.Type == EItemType::INVENTORY)
        {
            AItem_Inventory* Bag = Cast<AItem_Inventory>(AimedItem);

            if (Bag->InventoryType == EInventoryType::BOX)
            {
                return;
            }

            TArray<FSlotData>& Items = Bag->savedItems;

            if (Items.Num() > 0)
            {
                for (int i = 0; i < Items.Num(); i++)
                {
                    if (Items[i].bHaveItem)
                    {
                        return;
                    }
                }
            }


        }
        InventoryUI->AddItemEmptySlot(AimedItem);
        DestroyItemPerAimedItemType();
        //AimedItem->Destroy();
    }
}

void APlayerCharacter::CreateItem(const FInputActionValue& InputValue)
{
    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/BP_Item_bag.BP_Item_bag_C'"))) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    }

}

void APlayerCharacter::LeftTilt(const FInputActionValue& InputValue)
{
	if (bIsTilting || bRun || curState == PlayerState::DEAD)
		return;

	bIsTilting = true;
    TargetRoll = -15.0f;
    TargetOffset = FVector(0.0f, -10.0f, 0.0f);
}

void APlayerCharacter::UnLeftTilt(const FInputActionValue& InputValue)
{
    if (!bIsTilting || curState == PlayerState::DEAD)
        return;

	bIsTilting = false;
    TargetRoll = 0.0f;
    TargetOffset = FVector::ZeroVector;
}

void APlayerCharacter::RightTilt(const FInputActionValue& InputValue)
{
    if (bIsTilting || bRun || curState == PlayerState::DEAD)
        return;

	bIsTilting = true;
    TargetRoll = 15.0f;
    TargetOffset = FVector(0.0f, 10.0f, 0.0f);
}

void APlayerCharacter::UnRightTilt(const FInputActionValue& InputValue)
{
    if (!bIsTilting || curState == PlayerState::DEAD)
        return;

	bIsTilting = false;
    TargetRoll = 0.0f;
    TargetOffset = FVector::ZeroVector;
}

void APlayerCharacter::ToggleKeyTutorial(const FInputActionValue& InputValue)
{
    if (!KeyTutorialUI || !PlayerController || bOpenInventory) return;

    if (bIskeyTutorialActive)
    {
        KeyTutorialUI->HideCanvasPanel();
        bIskeyTutorialActive = false;

        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
    else
    {
        KeyTutorialUI->ShowCanvasPanel();
        bIskeyTutorialActive = true;

        PlayerController->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(KeyTutorialUI->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        PlayerController->SetInputMode(InputMode);
    }
}

void APlayerCharacter::CrouchStart(float Output)
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(Output);
}

void APlayerCharacter::CrouchEnd()
{
}

void APlayerCharacter::ToggleInventory(const FInputActionValue& InputValue)
{
    if (InventoryUI)
    {
        bOpenInventory = !bOpenInventory;
        InventoryUI->ToggleInventory(bOpenInventory);


        if (AimedItem && AimedItem->ItemData.Type == EItemType::INVENTORY && !InventoryUI->bWorldInventoryOpen)
        {
            InventoryUI->CreateWorldInventory(AimedItem);
        }
    }
}

void APlayerCharacter::StaminaControl()
{
    if (curState == PlayerState::RUN) {
        curStamina = FMath::Max(curStamina - timerRepeatTime * 0.1, 0.0f);

        if (FMath::IsNearlyZero(curStamina, 0.01f)) {
            PlayerUI->SetStaminaColor(FLinearColor::Red);
            curStamina = 0.f;
            Tired = true;

            UnRun(FInputActionValue());
        }
    }
    else {
        curStamina = FMath::Min(curStamina + timerRepeatTime * 0.1, 1.f);
        if (curStamina >= 0.2 && Tired) {
            Tired = false;
            PlayerUI->SetStamina(curStamina);
            PlayerUI->SetStaminaColor(FLinearColor::White);
        }
    }

    PlayerUI->SetStamina(curStamina);
}

void APlayerCharacter::HandStaminaControl()
{
    if (bAiming) {
        curHandStamina = FMath::Max(curHandStamina - timerRepeatTime * 0.1 , 0.0f);

        if (FMath::IsNearlyZero(curHandStamina, 0.01f)) {
            PlayerUI->SetHandStaminaColor(FLinearColor::Red);
            curHandStamina = 0.f;
            HandTired = true;

            UnAim(FInputActionValue());
        }
    }
    else {
        curHandStamina = FMath::Min(curHandStamina + timerRepeatTime * 0.1, 1.f);       
        
        if (curHandStamina >= 0.2f && HandTired) {
            HandTired = false;
            PlayerUI->SetHandStaminaColor(FLinearColor::White);
        }
    }


    PlayerUI->SetHandStamina(curHandStamina);
}

void APlayerCharacter::LoadInventoryClass()
{
    if (TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_PlayerInventoryWidget.BP_PlayerInventoryWidget_C'")))
    {
        InventoryUI = CreateWidget<UPlayerInventoryWidget>(GetWorld(), InventoryClass);
        if (InventoryUI)
        {
            InventoryUI->AddToViewport();
        }



        InventoryUI->ToggleInventory(bOpenInventory);
        InventoryUI->OnDropRequested.AddUObject(this, &APlayerCharacter::CreateInventoryItem);
        InventoryUI->OnItemUseRequested.AddUObject(this, &APlayerCharacter::UseItemWithDelay);
        InventoryUI->OnUpdateMagazineRequested.AddUObject(this, &APlayerCharacter::UpdateMagazine);

        InventoryUI->UpdateMagazine();
    }
}

void APlayerCharacter::CreateInventoryItem(FString name)
{
    FString FullPath = FString::Printf(TEXT("/Game/BluePrint/Item/BP_Item_%s.BP_Item_%s_C"), *name, *name);

    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, *FullPath)) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    }
}

void APlayerCharacter::UpdateItemUseDuration(float duration)
{
    ItemUseDuration += duration;

    if (ItemUseDuration >= ItemUseDelay) { UseItem(); }
    else { PlayerUI->UpdateItemUsingTime(ItemUseDuration / ItemUseDelay); }
}

void APlayerCharacter::UseItemWithDelay(FItemData data)
{
    bUsingItem = true;
    ItemUseDuration = 0.0f;

    ItemUseDelay = data.UseDelay;
    UsingItemData = data;

    PlayerUI->UpdateItemUsingTime(ItemUseDuration);
    PlayerUI->ShowUsingItemTimer();
}

void APlayerCharacter::UseItem()
{
    switch (UsingItemData.Type)
    {
    case EItemType::HEALING:

        Health = (Health + UsingItemData.Value > MaxHealth) ? MaxHealth : Health + UsingItemData.Value;
        PlayerUI->SetHealth(Health / MaxHealth);
        HitAndHealIndicatorUI->PlayHealHP();

        InventoryUI->SetHealth(Health / MaxHealth);

        break;

    case EItemType::ARMOR:

        Armor = (Armor + UsingItemData.Value > MaxArmor) ? MaxArmor : Armor + UsingItemData.Value;
        PlayerUI->SetArmor(Armor / MaxArmor);


        HitAndHealIndicatorUI->PlayHealArmor();

        InventoryUI->SetArmor(Armor / MaxArmor);
        break;

    case EItemType::AMMO:
        MaxAmmo += UsingItemData.Amount;
        PlayerUI->SetMagazineText(MaxAmmo);
        break;
    }

    bUsingItem = false;
    ItemUseDuration = 0.0f;
    PlayerUI->UpdateItemUsingTime(ItemUseDuration);
    PlayerUI->HideUsingItemTimer();
    InventoryUI->bUsingItem = false;
}

bool APlayerCharacter::IsEscaping()
{
    return bIsEscaping;
}

void APlayerCharacter::StartEscape()
{
    bIsEscaping = true;
    PlayerUI->ToggleEscapeCanvas(true);
    PlayerUI->UpdateEscapeTimer(EscapeTime);
    GetWorldTimerManager().SetTimer(LevelTimerHandle, this, &APlayerCharacter::ReturnToMain, EscapeTime, false);
}

void APlayerCharacter::StopEscape()
{
    bIsEscaping = false;
    EscapeDuration = 0.0f;
    UpdateEscapeDuration(0.0f);
    PlayerUI->ToggleEscapeCanvas(false);
}

void APlayerCharacter::UpdateEscapeDuration(float duration)
{
    EscapeDuration += duration;
    PlayerUI->UpdateEscapeTimer(EscapeTime - EscapeDuration);

    if (EscapeDuration >= EscapeTime)
    {
        InventoryUI->SaveInventories();
    }
}

void APlayerCharacter::ReturnToMain()
{
    if (curState == PlayerState::DEAD || bIsEscaping)
    {
        PlayFadeAndGoToMainLevel();
    }
}

void APlayerCharacter::PlayFadeAndGoToMainLevel()
{
    if (FadeInAndOut)
    {
        FadeInAndOut->PlayFadeIn();
    }

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        this, &APlayerCharacter::GoToMainLevel,
        5.f, false
    );
}

void APlayerCharacter::GoToMainLevel()
{
    UGameplayStatics::OpenLevel(this, FName("Main"));
}

void APlayerCharacter::UpdateMagazine(int maxAmmo)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("UpdateMagazine")));

    MaxAmmo = maxAmmo;
    PlayerUI->SetMagazineText(MaxAmmo);
}

void APlayerCharacter::GetPlayerAmmoData()
{
    //LoadSaveData
    //EquipAmmoArr ������Ʈ
    curAmmoIdx = 0;
    //�÷��̾� UI ������Ʈ
}

void APlayerCharacter::ChangeAmmo()
{
    if (curAmmoIdx + 1 > EquipAmmoArr.Num())
    {
        curAmmoIdx = 0;
    }

    else
    {
        curAmmoIdx += 1;
    }

    //ź�� ���� ���?
    //�÷��̾� UI ������Ʈ
}