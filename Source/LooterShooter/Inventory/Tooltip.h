// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


#include "Tooltip.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UTooltip : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Information;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bShouldFollowMouse = false;

};
