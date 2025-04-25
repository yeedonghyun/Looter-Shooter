// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LooterShooter/Item/ItemBase.h"

#include "Tooltip.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UTooltip : public UUserWidget
{
	GENERATED_BODY()

public:


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateTexts(FSlotData data, bool bActive);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Information;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Type;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Name;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Weight;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bShouldFollowMouse = false;

};
