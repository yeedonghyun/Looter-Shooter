#include "Bullet.h"
#include "UObject/ConstructorHelpers.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = true;

    InitialLifeSpan = 5.0f;
}

void ABullet::BeginPlay()
{
    Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
