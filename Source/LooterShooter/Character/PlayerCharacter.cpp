#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    CharacterMovement = GetCharacterMovement();
    Capsule = GetCapsuleComponent();
    Camera = nullptr;
    curState = PlayerState::IDEL;
    bCrouch = false;

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

    CrouchAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Crouch.IA_Crouch'"));

    RunCurve = LoadObject<UCurveFloat>(nullptr,
        TEXT("/Script/Engine.CurveFloat'/Game/Data/TimeLineCourve/RunFOV.RunFOV'"));

    CrouchCurve = LoadObject<UCurveFloat>(nullptr,
        TEXT("/Script/Engine.CurveFloat'/Game/Data/TimeLineCourve/CrouchCapsuleHalf.CrouchCapsuleHalf'"));
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    Camera = PlayerController->PlayerCameraManager;

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
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RunTimeline.TickTimeline(DeltaTime);
    CrouchTimeline.TickTimeline(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        FEnhancedInputActionHandlerValueSignature::TMethodPtr<APlayerCharacter> MethodPointer = &APlayerCharacter::Move;
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Jump;
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Look;
        EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Run;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, MethodPointer);
        
        MethodPointer = &APlayerCharacter::UnRun;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, MethodPointer);

        MethodPointer = &APlayerCharacter::Crouch;
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, MethodPointer);

        MethodPointer = &APlayerCharacter::UnCrouch;
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, MethodPointer);
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

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(-LookVector.Y);
}

void APlayerCharacter::Jump(const FInputActionValue& InputValue)
{
    ACharacter::Jump();
    curState = PlayerState::JUMP;
}

void APlayerCharacter::Run(const FInputActionValue& InputValue)
{
    if (!bCrouch && (curState == PlayerState::IDEL || curState == PlayerState::MOVEMENT))
    {
        curState = PlayerState::RUN;

        RunTimeline.Play();
        CharacterMovement->MaxWalkSpeed = 600;
    }
}

void APlayerCharacter::UnRun(const FInputActionValue& InputValue)
{
    RunTimeline.Reverse();
    CharacterMovement->MaxWalkSpeed = 300;

    FVector Velocity = GetVelocity();
    float Speed2D = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

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
    CrouchTimeline.Play();
    CharacterMovement->MaxWalkSpeed = 200;
    bCrouch = true;
}

void APlayerCharacter::UnCrouch(const FInputActionValue& InputValue)
{
    CrouchTimeline.Reverse();
    if (curState == RUN) {
        CharacterMovement->MaxWalkSpeed = 600;
    }
    else {
        CharacterMovement->MaxWalkSpeed = 300;
    }
    bCrouch = false;
}

void APlayerCharacter::CrouchStart(float Output)
{
    if (Capsule) 
    {
        Capsule->SetCapsuleHalfHeight(Output);
    }
}

void APlayerCharacter::CrouchEnd()
{
    UE_LOG(LogTemp, Log, TEXT("Crouch Timeline Finished!"));
}
