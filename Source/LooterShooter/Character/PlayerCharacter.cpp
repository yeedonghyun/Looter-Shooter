#include "PlayerCharacter.h"
#include "../Bullet/Bullet.h"

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
    MagazineAmmo = CurrentAmmo;
    AimedItem = nullptr;
    curHandStamina = 1.f;
    curStamina = 1.f;
    timerRepeatTime = 0.01;
    Tired = false;
    HandTired = false;

    bOpenInventory = false;

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

            PlayerUI->SetHandStamina(curHandStamina);
            PlayerUI->SetStamina(curStamina);
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

        InventoryUI->ToggleInventory(bOpenInventory);
        InventoryUI->OnDropRequested.AddUObject(this, &APlayerCharacter::CreateInventoryItem);

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

        // 인벤토리 백 업데이트
        if (InventoryUI)
        {
            if (InventoryUI->bOtherInventory)
            {
                InventoryUI->DeleteWorldInventory();
            }
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
    }
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    if (Controller && !bOpenInventory)
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
    if (curStamina < 0.1f || GetCharacterMovement()->IsFalling() || bOpenInventory) {
        return;
    }

    curStamina = FMath::Max(curStamina - 0.2f, 0.0f);
    PlayerUI->SetStamina(curStamina);

    Super::Jump();
    curState = PlayerState::JUMP;
}

void APlayerCharacter::Reload(const FInputActionValue& InputValue)
{
    if (bReload || bShoot || bOpenInventory) {
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
    if (Tired) {
        return;
    }

    if (bReload || bAiming || bCrouch || bOpenInventory || curState == PlayerState::IDLE) {
        return;
    }

    if (curState == PlayerState::MOVEMENT)
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
    curState = PlayerState::IDLE;
    bCrouch = false;
}

void APlayerCharacter::Aim(const FInputActionValue& InputValue)
{
    if (HandTired || bOpenInventory) {
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
    if (CurrentAmmo <= 0 || bShoot || bRun || bReload || bOpenInventory)
        return;

    if (PlayerUI) {
        CurrentAmmo--;
        PlayerUI->SetLeftAmmoText(CurrentAmmo);
    }

    SkeletalMeshComponent->GetAnimInstance()->Montage_Play(ShootAnimation, 1.f);
    Weapon->GetSkeletalMeshComponent()->GetAnimInstance()->Montage_Play(GunShootAnimation, 1.f);
    
    if (TSubclassOf<AActor> BulletClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_Bullet.BP_Bullet_C'")))    {

        if (BulletClass && Weapon) {
            FVector MuzzleLocation = Weapon->GetEndPointLocation();

            GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, Camera->GetCameraRotation());
        }
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

        if (AimedItem->ItemData.Type == EItemType::BAG)
        {
            AItem_bag* Bag = Cast<AItem_bag>(AimedItem);
            TArray<FSlotData>& Items = Bag->savedItems;

            for (int i = 0; i < Items.Num(); i++)
            {
                if (Items[i].bHaveItem)
                {
                    return;
                }
            }
        }
        InventoryUI->AddItemEmptySlot(AimedItem);
        //AimedItem->Destroy();
    }
}

void APlayerCharacter::CreateItem(const FInputActionValue& InputValue)
{
    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/BP_Item_bag.BP_Item_bag_C'"))) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    }

    //if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/Heal/BP_Bagg.BP_Bagg_C'"))) {
    //    AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    //}

    //if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/Heal/BP_Item_banage.BP_Item_banage_C'"))) {
    //    AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    //}
}

void APlayerCharacter::CrouchStart(float Output)
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(Output);
}

void APlayerCharacter::CrouchEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Crouch Timeline Finished!"));
}

void APlayerCharacter::ToggleInventory(const FInputActionValue& InputValue)
{
    if (InventoryUI)
    {
        bOpenInventory = !bOpenInventory;
        InventoryUI->ToggleInventory(bOpenInventory);


        if (AimedItem && AimedItem->ItemData.Type == EItemType::BAG && !InventoryUI->bOtherInventory)
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


void APlayerCharacter::CreateInventoryItem(FString name)
{
    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/BP_Item_bag.BP_Item_bag_C'"))) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, GunEndPoint, Camera->GetCameraRotation());
    }
}