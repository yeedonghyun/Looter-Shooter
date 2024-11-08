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

public:
    USelectMapUserWidget(const FObjectInitializer& Object);

private:

    UPROPERTY(meta = (BindWidget))
    UButton* Play;

    UPROPERTY(meta = (BindWidget))
    UButton* AsianTown;

    UPROPERTY(meta = (BindWidget))
    UButton* MilitaryAirport;

    UPROPERTY(meta = (BindWidget))
    UButton* Back;

    UFUNCTION()
    void OnPlayButtonClicked();

    UFUNCTION()
    void OnAsianTownButtonClicked();

    UFUNCTION()
    void OnMilitaryAirportButtonClicked();

    UFUNCTION()
    void OnBackButtonClicked();

    UFUNCTION()
    void OnPlayButtonHovered();

    UFUNCTION()
    void OnPlayButtonUnhovered();

    UFUNCTION()
    void OnAsianTownButtonHovered();

    UFUNCTION()
    void OnAsianTownButtonUnhovered();

    UFUNCTION()
    void OnMilitaryAirportButtonHovered();

    UFUNCTION()
    void OnMilitaryAirportButtonUnhovered();

    UFUNCTION()
    void OnExitButtonHovered();

    UFUNCTION()
    void OnExitButtonUnhovered();

private:
    int MapIndex;
    FLinearColor color;
};
