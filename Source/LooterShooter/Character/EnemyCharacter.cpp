#include "EnemyCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PhysicsEngine/BodyInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    MaxDetectionRange = 3000;
    MaxDetectionAngle = 180;

    FireRate = 0.4f;
    DetecteRate = 0.1f;
    bDetectPlayer = false;
    CurrentAmmo = 30;
    bShoot = false;
    bRecentDetectPlayer = false;
    Health = 100;
	bIsDead = false;

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
        }

        if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Component))
        {
            if (ChildActorComp->ComponentHasTag(TEXT("Inventory")))
            {
                AItem_Inventory* InventoryActor = Cast<AItem_Inventory>(ChildActorComp->GetChildActor());
                if (InventoryActor)
                {
                    inventroy = InventoryActor;
                }
            }
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

    if (!bIsDead && IsDetectPlayer())
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->StopMovement();            
        }

        if (TargetPlayer && bDetectPlayer)
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
        return false;
    }

    FVector EnemyLocation = GetActorLocation();
    FVector PlayerLocation = TargetPlayer->GetActorLocation();

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        EnemyLocation,
        PlayerLocation,
		//ECC_Visibility,
        ECC_WorldDynamic,
        Params
    );

    if ((!bHit || !Hit.GetActor()->ActorHasTag("Player")) && !Hit.GetActor()->ActorHasTag("IgnoreDetection")) {
        if (bDetectPlayer) {
            bRecentDetectPlayer = true;
            GetWorldTimerManager().SetTimer(DetectPlayerTimerHandle, this, &AEnemyCharacter::CheckRecentlyDetectPlayer, 0.4f, false);
        }
        bDetectPlayer = false;
        return false;
    }

    FVector ToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();
    FVector Forward = GetActorForwardVector();

    float Distance = FVector::Dist(EnemyLocation, PlayerLocation);
    float Dot = FVector::DotProduct(Forward, ToPlayer);
    float Angle = FMath::Acos(Dot) * (180.f / PI);

    if (Distance > MaxDetectionRange || Angle > MaxDetectionAngle) {
        if (bDetectPlayer) {
            bRecentDetectPlayer = true;
            GetWorldTimerManager().SetTimer(DetectPlayerTimerHandle, this, &AEnemyCharacter::CheckRecentlyDetectPlayer, 0.4f, false);
        }
        bDetectPlayer = false;
        return false;
    }

    bDetectPlayer = true;
    LastKnownPlayerLocation = PlayerLocation;
    return true;
}

void AEnemyCharacter::CheckRecentlyDetectPlayer()
{
    if (!bDetectPlayer && bRecentDetectPlayer) {
        LostPlayer = true;
        bRecentDetectPlayer = false;
    }
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

bool AEnemyCharacter::MoveToLocation(FVector TargetLocation, float AcceptanceRadius)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (!AIController) return false;

    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation, AcceptanceRadius);

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
		return false;
    }
    else
    {
        return true;
    }
}

bool AEnemyCharacter::MoveToTarget(AActor* TargetActor, float AcceptanceRadius)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (!TargetActor || !AIController) return false;

    FVector TargetLocation = TargetActor->GetActorLocation();

    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(TargetLocation, AcceptanceRadius);

    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        return false;
    }
    else
    {
        return true;
    }
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

bool AEnemyCharacter::Fire()
{
    if (!bDetectPlayer || bShoot)
        return false;

    if (BulletClass && Weapon) {
        FVector MuzzleLocation = Weapon->GetEndPointLocation();

        float RandomYaw = FMath::RandRange(-5.0f, 5.0f);
        float RandomPitch = FMath::RandRange(-3.0f, 3.0f);

        FRotator RandomizedRotation = GetActorRotation();
        RandomizedRotation.Yaw += RandomYaw;
        RandomizedRotation.Pitch += RandomPitch;

        GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, RandomizedRotation);
    }

    bShoot = true;

    GetWorldTimerManager().SetTimer(ShootResetTimerHandle, this, &AEnemyCharacter::ResetShoot, FireRate, false);

    return true;
}

void AEnemyCharacter::ResetShoot()
{
    bShoot = false;
}

void AEnemyCharacter::ApplyDamage(int DamageAmount)
{
    Health -= DamageAmount;
    if (Health > 0 || bIsDead) return;

    bIsDead = true;
    bDetectPlayer = false;

    if (Weapon) Weapon->Destroy();

    UCapsuleComponent* Capsule = GetCapsuleComponent();
    Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (SkeletalMeshComponent)
    {
        SkeletalMeshComponent->bBlendPhysics = true;
        SkeletalMeshComponent->bPauseAnims = true;

        SkeletalMeshComponent->SetSimulatePhysics(true);
        SkeletalMeshComponent->SetAllBodiesSimulatePhysics(true);
        SkeletalMeshComponent->WakeAllRigidBodies();

        SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
        SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
        SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    }

    if (AAIController* AIC = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
        {
            BB->SetValueAsBool(TEXT("bIsDead"), true);			
        }
        AIC->UnPossess();
    }

    GetWorldTimerManager().SetTimer(FreezeStateTimerHandle, this,
        &AEnemyCharacter::FreezeRagdoll, 2.0f, false);

    CurrentState = EEnemyState::Dead;
}

void AEnemyCharacter::FreezeRagdoll()
{
    if (!SkeletalMeshComponent) return;

    // 모든 속도 정지
    SkeletalMeshComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector, false);
    SkeletalMeshComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

    // 시뮬레이션 비활성화
    SkeletalMeshComponent->SetSimulatePhysics(false);

    // 루트 본 강제 위치 고정
    SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

    // 애니메이션 정지
    SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    SkeletalMeshComponent->bPauseAnims = true;
    SkeletalMeshComponent->SetComponentTickEnabled(false);
}

void AEnemyCharacter::CreateInventoryItem(FString name)
{
    //FVector SpawnLocation = FVector(-17307.0, 3811.0, 1222.100393);
    //FRotator SpawnRotation = FRotator(22.833177, 96.678432, 5.169551);

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    if (TSubclassOf<AActor> TestItemClass = LoadClass<AActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/BluePrint/Item/BP_Item_box36.BP_Item_box36_C'"))) {
        AItemBase* SpawnedBullet = GetWorld()->SpawnActor<AItemBase>(TestItemClass, SpawnLocation, SpawnRotation);
    }
}

AItem_Inventory* AEnemyCharacter::GetInventory()
{
    return inventroy;
}
