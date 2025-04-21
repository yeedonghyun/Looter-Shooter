#include "EnemyCharacter.h"
#include "AIController.h"
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

bool AEnemyCharacter::MoveToLocation(FVector Location, float DeltaTime)
{
    float Distance = FVector::Dist(Location, GetActorLocation());
    if (Distance < 1.f)
    {
        return true;
    }

    FVector Direction = (Location - GetActorLocation()).GetSafeNormal();
    FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    SetActorRotation(NewRotation);

    if (GetMovementComponent())
    {
        float Speed = GetMovementComponent()->GetMaxSpeed();
        FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
        SetActorLocation(NewLocation);
    }

    return false;
}

bool AEnemyCharacter::Rotate(float Degree, float RotationSpeed)
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
    return YawDiff < 1.f;
}

bool AEnemyCharacter::RotateToTarget(AActor* TargetActor, float RotationSpeed)
{
    if (!TargetActor) return false;

    FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    float TargetYaw = Direction.Rotation().Yaw;
    float CurrentYaw = GetActorRotation().Yaw;

    float NewYaw = FMath::FixedTurn(CurrentYaw, TargetYaw, RotationSpeed * GetWorld()->GetDeltaSeconds());
    FRotator NewRotation(0.f, NewYaw, 0.f);
	SetActorRotation(NewRotation);

    float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(NewYaw, TargetYaw));
    return YawDiff < 1.f;
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