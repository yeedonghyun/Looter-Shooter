#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitAndHealIndicatorWidget.generated.h"

UCLASS()
class LOOTERSHOOTER_API UHitAndHealIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Hit;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HealHP;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HealArmor;

public:
	UFUNCTION(BlueprintCallable)
	void PlayHit();

	UFUNCTION(BlueprintCallable)
	void PlayHealHP();

	UFUNCTION(BlueprintCallable)
	void PlayHealArmor();
};
