#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "SelectMapUserWidget.generated.h"

UCLASS()
class LOOTERSHOOTER_API USelectMapUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    UButton* Play;

    UPROPERTY(meta = (BindWidget))
    UButton* Storage;

    UPROPERTY(meta = (BindWidget))
    UButton* Setting;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitGame;

    UFUNCTION()
    void OnPlayButtonClicked();

    UFUNCTION()
    void OnStorageButtonClicked();

    UFUNCTION()
    void OnSettingButtonClicked();

    UFUNCTION()
    void OnExitGameButtonClicked();

    UFUNCTION()
    void OnPlayButtonHovered();

    UFUNCTION()
    void OnPlayButtonUnhovered();

    UFUNCTION()
    void OnStorageButtonHovered();

    UFUNCTION()
    void OnStorageButtonUnhovered();

    UFUNCTION()
    void OnSettingButtonHovered();

    UFUNCTION()
    void OnSettingButtonUnhovered();

    UFUNCTION()
    void OnExitButtonHovered();

    UFUNCTION()
    void OnExitButtonUnhovered();
};
