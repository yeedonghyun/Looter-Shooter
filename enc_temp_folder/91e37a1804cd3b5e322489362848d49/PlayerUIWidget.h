#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "PlayerUIWidget.generated.h"

UCLASS()
class LOOTERSHOOTER_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Aim;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Magazine;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* UHandStamina;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* UStamina;

public:
	virtual void NativeConstruct() override;

	void HideCrosshairOnAim();
	void ShowCrosshairOnAimEnd();

	void SetLeftAmmoText(int CurAmmo);

	void SetHandStamina(float Stamina);
	void SetStamina(float Stamina);
};
