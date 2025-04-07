#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bShoot = false;
    MaxDetectionRange = 3000;
    MaxDetectionAngle = 180;

    RotationSpeed = 10;
    FireRate = 0.2f;
    DetecteRate = 0.1f;
    bSeePlayer = false;

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

    GetWorldTimerManager().SetTimer(DetecteTimerHandle, this, &AEnemyCharacter::DetectePlayer, DetecteRate, true);

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

void AEnemyCharacter::DetectePlayer()
{
	IsSeePlayer();
    Fire();    
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RotateToPlayer(DeltaTime);
}

void AEnemyCharacter::IsSeePlayer()
{
    if (!TargetPlayer) return;

    FVector EnemyLocation = GetActorLocation();
    FVector PlayerLocation = TargetPlayer->GetActorLocation();
    FVector ToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
    FVector Forward = GetActorForwardVector();

    float Distance = FVector::Dist(EnemyLocation, PlayerLocation);
    if (Distance > MaxDetectionRange) {
		bSeePlayer = false;
        return;
    }

    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);

    if (Angle > MaxDetectionAngle) {
        bSeePlayer = false;
        return;
    }

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, EnemyLocation, PlayerLocation, ECC_Visibility, Params);

    bSeePlayer = true;
}

void AEnemyCharacter::RotateToPlayer(float DeltaTime)
{
    if (!TargetPlayer) return;

    FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    FRotator CurrentRotation = GetActorRotation();
    float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
    float TurnAmount = FMath::Clamp(DeltaYaw, -RotationSpeed * DeltaTime, RotationSpeed * DeltaTime);

    CurrentRotation.Yaw += TurnAmount;
    SetActorRotation(CurrentRotation);
}


void AEnemyCharacter::Fire()
{
    if (!bSeePlayer || bShoot || !IsFacingPlayer())
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

bool AEnemyCharacter::IsFacingPlayer()
{
    if (!TargetPlayer) return false;

    FVector Forward = GetActorForwardVector();
    FVector ToPlayer = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);

    return Angle < 10.0f;
}
