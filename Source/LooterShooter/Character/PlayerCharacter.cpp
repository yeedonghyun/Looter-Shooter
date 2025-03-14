#include "PlayerCharacter.h"
#include "../Bullet/Bullet.h"

APlayerCharacter::APlayerCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    curState = PlayerState::IDEL;
    bCrouch = false;
    bAiming = false;
    bWallCloseInFront = false;
    bRun = false;
    bSemiFire = false;
    Sensitivity = 0.4;
    CurrentAmmo = 30;
    MagazineAmmo = CurrentAmmo;
    AimedItem = nullptr;

    GunEndPoint = FVector(0.f, 0.f, 0.f);

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

    if (RunCurve != nullptr)
    {
        RunTimeLineUpdateDelegate.BindUFunction(this, FName("RunStart"));
        RunTimeLineFinishDelegate.BindUFunction(this, FName("RunEnd"));

        RunTimeline.AddInterpFloat(RunCurve, RunTimeLineUpdateDelegate);
        RunTimeline.SetTimelineFinishedFunc(RunTimeLineFinishDelegate);
    }

    if (CrouchCurve != nullptr) 
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
        }
    }

    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("SkeletalMeshComponent")))
        {
            SkeletalMeshComponent = Cast<USkeletalMeshComponent>(Component);
            break;
        }
    }

    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector SpawnLocation = FVector::ZeroVector;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        AActor* SpawnedWeapon = GetWorld()->SpawnActor<AActor>(
           WeaponClass,
            SpawnLocation,
            SpawnRotation,
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


    if (TSubclassOf<UUserWidget> InventoryClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_PlayerInventoryWidget.BP_PlayerInventoryWidget_C'")))
    {
        InventoryUI = CreateWidget<UPlayerInventoryWidget>(GetWorld(), InventoryClass);
        if (InventoryUI)
        {
            InventoryUI->AddToViewport();
        }

        InventoryUI->CloseInventory();
    }

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RunTimeline.TickTimeline(DeltaTime);
    CrouchTimeline.TickTimeline(DeltaTime);

    auto CamearaRotation = PlayerController->GetControlRotation();
    PivotComponent->SetRelativeRotation(FRotator(CamearaRotation.Pitch, 0, 0));

    CheckObjectCloseAhead();
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
    FVector EndPoint = ((Rotation.Vector() * ViewDis) + Start);
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(this); 

    bool bCollision = GetWorld()->LineTraceSingleByChannel(HitOut, Start, EndPoint, ECC_Visibility, TraceParams);

    if (bCollision)
    {
        AActor* HitActor = HitOut.GetActor();
        if (HitActor && HitActor->IsA(AItemBase::StaticClass())) 
        {
            AimedItem = Cast<AItemBase>(HitActor);
            if (PlayerUI)
            {
                PlayerUI->ShowCrosshairOnAimEnd();
            }
        }
        else
        {
            AimedItem = nullptr;
            if (PlayerUI)
            {
                PlayerUI->HideCrosshairOnAim();
            }
        }
    }
    else
    {
        AimedItem = nullptr;
        if (PlayerUI)
        {
            PlayerUI->HideCrosshairOnAim();
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

        MethodPointer = &APlayerCharacter::Look;
        EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Jump;
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, MethodPointer);

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
        EnhancedInputComponent->BindAction(PickUpItemAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::CreateItem;
        EnhancedInputComponent->BindAction(CreateItemAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::InventoryOnOrOff;
        EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, MethodPointer);
    }
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    if (Controller)
    {
        curState = PlayerState::MOVEMENT;

        FVector2D MovementVector = InputValue.Get<FVector2D>();
        const FVector Forward = GetActorForwardVector();
        const FVector Right = GetActorRightVector();

        const FVector Direction = (Forward * MovementVector.Y) + (Right * MovementVector.X);

        AddMovementInput(Direction, 1.0f);
    }
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
    FVector2D LookVector = InputValue.Get<FVector2D>();

    AddControllerYawInput(LookVector.X * Sensitivity);
    AddControllerPitchInput(-LookVector.Y * Sensitivity);
}

void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    ACharacter::Jump();
    curState = PlayerState::JUMP;
}

void APlayerCharacter::Reload(const FInputActionValue& InputValue)
{
    if (bReload || bShoot) {
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
    if (PlayerUI) {
        CurrentAmmo = MagazineAmmo;
        PlayerUI->SetLeftAmmoText(CurrentAmmo);
    }

    bReload = false;
}

void APlayerCharacter::Run(const FInputActionValue& InputValue)
{
    if (bReload || bAiming || bCrouch) {
        return;
    }

    if (curState == PlayerState::IDEL || curState == PlayerState::MOVEMENT)
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
        curState = PlayerState::IDEL;
    }
}

void APlayerCharacter::RunStart(float Output)
{
    if (Camera)
    {
        Camera->SetFOV(Output);
    }
}

void APlayerCharacter::RunEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Run Timeline Finished!"));
}

void APlayerCharacter::Crouch(const FInputActionValue& InputValue)
{
    if (bRun) {
        UnRun(InputValue);
    }

    CrouchTimeline.Play();
    RunTimeline.Reverse();
    GetCharacterMovement()->MaxWalkSpeed = 200;
    bCrouch = true;
}

void APlayerCharacter::UnCrouch(const FInputActionValue& InputValue)
{
    CrouchTimeline.Reverse();
    GetCharacterMovement()->MaxWalkSpeed = 300;
    curState = PlayerState::IDEL;
    bCrouch = false;
}

void APlayerCharacter::Aim(const FInputActionValue& InputValue)
{
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
    if (CurrentAmmo <= 0 || bShoot || bRun || bReload)
        return;

    if (PlayerUI) {
        CurrentAmmo--;
        PlayerUI->SetLeftAmmoText(CurrentAmmo);
    }

    SkeletalMeshComponent->GetAnimInstance()->Montage_Play(ShootAnimation, 1.f);
    Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Play(GunShootAnimation, 1.f);
    
    if (TSubclassOf<AActor> BulletClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_Bullet.BP_Bullet_C'")))    {
        ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GunEndPoint, Camera->GetCameraRotation());
    }

    bShoot = true;
    
    float AnimationDuration = 0.2;
    GetWorldTimerManager().SetTimer(ShootResetTimerHandle, this, &APlayerCharacter::ResetShoot, AnimationDuration, false);
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

        InventoryUI->AddItem(*AimedItem);

        AimedItem->Destroy();
    }
}

void APlayerCharacter::CreateItem(const FInputActionValue& InputValue)
{
    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/Heal/BP_Item_banage.BP_Item_banage_C'"))) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    }
}

void APlayerCharacter::CrouchStart(float Output)
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(Output);
}

void APlayerCharacter::CrouchEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Crouch Timeline Finished!"));
}

void APlayerCharacter::InventoryOnOrOff(const FInputActionValue& InputValue)
{
    if (InventoryUI != nullptr)
    {
        if (InventoryUI->Visibility == ESlateVisibility::Hidden)
        {
            InventoryUI->OpenInventory();
        }

        else
        {
            InventoryUI->CloseInventory();
        }
    }
}