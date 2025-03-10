#include "Item_banage.h"

void AItem_banage::BeginPlay()
{
	Super::BeginPlay();

    TArray<UActorComponent*> Components;
    GetComponents(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->ComponentHasTag(TEXT("StaticlMeshComponent")))
        {
            StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
            break;
        }
    }
}