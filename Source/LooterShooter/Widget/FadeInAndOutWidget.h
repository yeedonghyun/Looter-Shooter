#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeInAndOutWidget.generated.h"

UCLASS()
class LOOTERSHOOTER_API UFadeInAndOutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Loading;

public:
	UFUNCTION(BlueprintCallable)
	void PlayFadeIn();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOut();

	UFUNCTION(BlueprintCallable)
	void PlayLoading();
};
