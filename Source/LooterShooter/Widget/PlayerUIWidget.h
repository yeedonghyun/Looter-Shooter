#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlayerUIWidget.generated.h"

UCLASS()
class LOOTERSHOOTER_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Aim;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Magazine;

	virtual void NativeConstruct() override;

	void HideCrosshairOnAim();
	void ShowCrosshairOnAimEnd();

	void SetLeftAmmoText(int CurAmmo);
};
