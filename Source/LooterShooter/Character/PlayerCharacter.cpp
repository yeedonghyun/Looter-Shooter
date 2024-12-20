#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    CharacterMovement = GetCharacterMovement();

    IMC = LoadObject<UInputMappingContext>(nullptr, 
        TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Data/IMC_FPS.IMC_FPS'")); 
    
    MovementAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Movement.IA_Movement'"));

    CameraAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Camera.IA_Camera'"));

    RunAction = LoadObject<UInputAction>(nullptr,
        TEXT("/Script/EnhancedInput.InputAction'/Game/Data/InputAction/IA_Run.IA_Run'"));

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                InputSubsystem->AddMappingContext(IMC, 0);
            }
        }
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
    if (Controller)
    {
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

void APlayerCharacter::TriggeredRun(const FInputActionValue& InputValue)
{
    CharacterMovement->MaxWalkSpeed = 1200;
}

void APlayerCharacter::CompletedRun(const FInputActionValue& InputValue)
{
    CharacterMovement->MaxWalkSpeed = 600;
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

        MethodPointer = &APlayerCharacter::TriggeredRun;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, MethodPointer);
        
        MethodPointer = &APlayerCharacter::CompletedRun;
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, MethodPointer);
    }
}

