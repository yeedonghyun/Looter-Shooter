#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Math/Color.h"
#include "Components/EditableTextBox.h"
#include <Components/CanvasPanel.h>

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


	UPROPERTY(meta = (BindWidget))
	UProgressBar* UHealth;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* UArmor;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* UUsingItemTime;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* EscapeCanvas;

public:
	virtual void NativeConstruct() override;

	void HideCrosshairOnAim();
	void ShowCrosshairOnAimEnd();

	void SetLeftAmmoText(int CurAmmo);

	void SetStamina(float Stamina);
	void SetHandStamina(float Stamina);

	void SetStaminaColor(FLinearColor color);
	void SetHandStaminaColor(FLinearColor color);


	void SetHealth(float value);
	void SetArmor(float value);

	void UpdateTimerUI(float Time);
	void UpdateItemUsingTime(float value);

	void HideUsingItemTimer();
	void ShowUsingItemTimer();


	void UpdateEscapeTimer(float Time);
	void ToggleEscapeCanvas(bool isUse);


	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* GameTimer;


	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EscapeTimer;


	

};
