// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragDropSlot.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UDragDropSlot : public UDragDropOperation
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnywhere)
		int PrevSlotIndex;
};
