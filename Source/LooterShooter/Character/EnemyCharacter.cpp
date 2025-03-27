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
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    BulletClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Bullet/BP_Bullet.BP_Bullet_C'"));
    TargetPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    GetWorldTimerManager().SetTimer(DetecteTimerHandle, this, &AEnemyCharacter::DetectePlayer, DetecteRate, true);
}

void AEnemyCharacter::DetectePlayer()
{
    if (CanSeePlayer())
    {
        RotateToPlayer();
        Fire();
    }
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CachedDeltaTime = DeltaTime;
}

bool AEnemyCharacter::CanSeePlayer()
{
    if (!TargetPlayer) return false;

    FVector EnemyLocation = GetActorLocation();
    FVector PlayerLocation = TargetPlayer->GetActorLocation();
    FVector ToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
    FVector Forward = GetActorForwardVector();

    float Distance = FVector::Dist(EnemyLocation, PlayerLocation);
    if (Distance > MaxDetectionRange) return false;

    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);

    if (Angle > MaxDetectionAngle) return false;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, EnemyLocation, PlayerLocation, ECC_Visibility, Params);

    return !bBlocked || HitResult.GetActor() == TargetPlayer;
}

void AEnemyCharacter::RotateToPlayer()
{
    if (!TargetPlayer) return;

    FVector Direction = (TargetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FRotator TargetRotation = Direction.Rotation();

    FRotator CurrentRotation = GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, CachedDeltaTime, RotationSpeed);

    SetActorRotation(NewRotation);
}

void AEnemyCharacter::Fire()
{
    if (!CanSeePlayer() || bShoot || !IsFacingPlayer())
        return;

    if (BulletClass) {
        FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * 50.f;
        FRotator MuzzleRotation = GetActorRotation();

        GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation);
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