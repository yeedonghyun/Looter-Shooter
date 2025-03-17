#include "PlayerUIWidget.h"

void UPlayerUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeftAmmo)
    {
        LeftAmmo->SetText(FText::FromString(TEXT("30 / ")));
    }

    if (Magazine)
    {
        Magazine->SetText(FText::FromString(TEXT("30")));
    }
}

void UPlayerUIWidget::HideCrosshairOnAim()
{
    if (Aim)
        Aim->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUIWidget::ShowCrosshairOnAimEnd()
{
    if (Aim)
        Aim->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUIWidget::SetLeftAmmoText(int CurAmmo)
{
    FString AmmoString = FString::FromInt(CurAmmo) + TEXT(" / ");
    LeftAmmo->SetText(FText::FromString(AmmoString));
}

void UPlayerUIWidget::SetHandStamina(float Stamina)
{
    UHandStamina->SetPercent(Stamina);
}

void UPlayerUIWidget::SetStamina(float Stamina)
{
    UStamina->SetPercent(Stamina);
}

