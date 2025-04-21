#include "SoapBubbleBullet.h"

ASoapBubbleBullet::ASoapBubbleBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 5.0f;
}

void ASoapBubbleBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoapBubbleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

