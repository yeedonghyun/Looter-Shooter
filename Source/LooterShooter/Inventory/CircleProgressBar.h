// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "CircleProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UCircleProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
	float Progress = 0.5f;

	UFUNCTION(BlueprintImplementableEvent, Category = "MyFuncs")
	void SetPercentage(float f); // 블루프린트에서 구현할 함수


};
