#include "EnemyCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxDetectionRange = 3000;
    MaxDetectionAngle = 180;

    FireRate = 0.2f;
    DetecteRate = 0.1f;
    bSeePlayer = false;
    CurrentAmmo = 30;
    bShoot = false;

    CurrentState = EEnemyState::Idle;

    static ConstructorHelpers::FClassFinder<AActor> WeaponBP(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Gun/BP_Weapon1.BP_Weapon1_C'"));
    if (WeaponBP.Succeeded())
    {
        WeaponClass = WeaponBP.Class;
    }
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    BulletClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_Bullet.BP_Bullet_C'"));
    TargetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

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

        FVector SpawnLocation = FVector(-11.711598, 5.042916, 4.060307);
        FRotator SpawnRotation = FRotator(22.833177, 96.678432, 5.169551);

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
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsDetectPlayer())
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->StopMovement();            
        }

        if (TargetPlayer && bSeePlayer)
        {
            RotateToTarget(TargetPlayer, 5.0f);
        }
    }
}

void AEnemyCharacter::UpdateWalkSpeed(float NewWalkSpeed)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
    }
}

bool AEnemyCharacter::IsDetectPlayer()
{
    if (!TargetPlayer) {
        bSeePlayer = false;
        LostPlayer = false;
        return false;
    }

    FVector EnemyLocation = GetActorLocation();
    FVector PlayerLocation = TargetPlayer->GetActorLocation();

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        EnemyLocation,
        PlayerLocation,
        FQuat::Identity,
        ECC_WorldDynamic,
        FCollisionShape::MakeSphere(50.f),
        Params
    );

    if (FVector::Dist(LastKnownPlayerLocation, GetActorLocation()) < 50.f) {
        LostPlayer = false;
    }

    if (!bHit || !Hit.GetActor()->ActorHasTag("Player")) {
        if (bSeePlayer) {
            LastKnownPlayerLocation = TargetPlayer->GetActorLocation();
            LostPlayer = true;
        }
        bSeePlayer = false;
        return false;
    }

    FVector ToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
    FVector Forward = GetActorForwardVector();

    float Distance = FVector::Dist(EnemyLocation, PlayerLocation);
    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);

    if (Distance > MaxDetectionRange || Angle > MaxDetectionAngle) {
        if (bSeePlayer) {
            LastKnownPlayerLocation = TargetPlayer->GetActorLocation();
            LostPlayer = true;
        }
        bSeePlayer = false;
        return false;
    }

    bSeePlayer = true;
    return true;
}

bool AEnemyCharacter::IsAimedPlayer()
{
    if (!TargetPlayer) 
        return false;
    
    FVector Forward = GetActorForwardVector();
    FVector ToPlayer = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    
    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);
    
    return Angle < 10.0f;
}

void AEnemyCharacter::MoveToLocation(FVector TargetLocation, float AcceptanceRadius)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation, AcceptanceRadius);
}

void AEnemyCharacter::MoveToTarget(AActor* TargetActor, float AcceptanceRadius)
{
    if (!TargetActor) return;

    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return;

    FVector TargetLocation = TargetActor->GetActorLocation();

    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation, AcceptanceRadius);
}

void AEnemyCharacter::Rotate(float Degree, float RotationSpeed)
{
    FRotator CurrentRotation = GetActorRotation();
    CurrentRotation.Yaw += RotationSpeed * GetWorld()->GetDeltaSeconds();
    if (CurrentRotation.Yaw >= Degree)
    {
        CurrentRotation.Yaw -= Degree;
    }

    SetActorRotation(CurrentRotation);

	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, Degree, CurrentRotation.Roll);

    float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw));
}

void AEnemyCharacter::RotateToTarget(AActor* TargetActor, float RotationSpeed)
{
    if (!TargetActor) return;

    FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    FRotator CurrentRotation = GetActorRotation();

    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

    SetActorRotation(NewRotation);
}

void AEnemyCharacter::Fire()
{
    if (!bSeePlayer || bShoot)
        return;

    if (BulletClass && Weapon) {
        FVector MuzzleLocation = Weapon->GetEndPointLocation();

        GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, GetActorRotation());
    }

    bShoot = true;

    GetWorldTimerManager().SetTimer(ShootResetTimerHandle, this, &AEnemyCharacter::ResetShoot, FireRate, false);
}

void AEnemyCharacter::ResetShoot()
{
    bShoot = false;
}