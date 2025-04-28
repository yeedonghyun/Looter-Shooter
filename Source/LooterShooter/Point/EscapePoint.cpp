#include "EscapePoint.h"
#include "../Character/PlayerCharacter.h"
#include "Components/BoxComponent.h"

AEscapePoint::AEscapePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;
	EscapeTime = 5.0f;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EscapeBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);       
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);
}

void AEscapePoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEscapePoint::OverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AEscapePoint::OverlapEnd);
}

void AEscapePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEscapePoint::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin with: %s"), *OtherActor->GetName());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		{
			//if (PlayerCharacter)
			//{
			//	//PlayerCharacter->StartEscape(EscapeTime);
			//}
		}
	}
}

void AEscapePoint::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap End with: %s"), *OtherActor->GetName());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		{
			//if (PlayerCharacter && PlayerCharacter->IsEscaping())
			//{
			//	PlayerCharacter->EndEscape(EscapeTime);
			//}
		}
	}
}