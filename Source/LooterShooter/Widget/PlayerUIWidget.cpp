#include "PlayerUIWidget.h"

void UPlayerUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeftAmmo)
    {
        LeftAmmo->SetText(FText::FromString(TEXT("30")));
        //LeftAmmo->SetText(FText::FromString(TEXT("30 / ")));
    }

    if (Magazine)
    {
        Magazine->SetText(FText::FromString(TEXT("30")));
    }

    HideUsingItemTimer();

    ToggleEscapeCanvas(false);
    ToggleInfoUI(false);
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
    FString AmmoString = FString::FromInt(CurAmmo);
    //FString AmmoString = FString::FromInt(CurAmmo) + TEXT(" / ");
    LeftAmmo->SetText(FText::FromString(AmmoString));
}

void UPlayerUIWidget::SetStamina(float Stamina)
{
    UStamina->SetPercent(Stamina);
}

void UPlayerUIWidget::SetHandStamina(float Stamina)
{
    UHandStamina->SetPercent(Stamina);
}

void UPlayerUIWidget::SetStaminaColor(FLinearColor color)
{
    UStamina->SetFillColorAndOpacity(color);
}

void UPlayerUIWidget::SetHandStaminaColor(FLinearColor color)
{
    UHandStamina->SetFillColorAndOpacity(color);
}


void UPlayerUIWidget::SetHealth(float value)
{
    UHealth->SetPercent(value);
}

void UPlayerUIWidget::SetArmor(float value)
{
    UArmor->SetPercent(value);
}


void UPlayerUIWidget::HideUsingItemTimer()
{
    UUsingItemTime->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUIWidget::ShowUsingItemTimer()
{
    UUsingItemTime->SetVisibility(ESlateVisibility::Visible);
}



void UPlayerUIWidget::UpdateItemUsingTime(float value)
{
    UUsingItemTime->SetPercent(value);
}

void UPlayerUIWidget::UpdateTimerUI(float Time)
{
    int32 TotalSeconds = FMath::FloorToInt(Time);
    int32 Hours = TotalSeconds / 3600;
    int32 Minutes = (TotalSeconds % 3600) / 60;
    int32 Seconds = TotalSeconds % 60;

    FString FormattedTime = FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);


    if (GameTimer)
    {
        GameTimer->SetText(FText::FromString(FormattedTime));
    }

}

void UPlayerUIWidget::ToggleEscapeCanvas(bool isUse)
{
    if (isUse)
    {
        EscapeCanvas->SetVisibility(ESlateVisibility::Visible);
    }

    else
    {
        EscapeCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
}


void UPlayerUIWidget::UpdateEscapeTimer(float Time)
{
    int32 TotalSeconds = FMath::FloorToInt(Time);
    int32 Hours = TotalSeconds / 3600;
    int32 Minutes = (TotalSeconds % 3600) / 60;
    int32 Seconds = TotalSeconds % 60;

    FString FormattedTime = FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);


    if (EscapeTimer)
    {
        EscapeTimer->SetText(FText::FromString(FormattedTime));
    }

}

void UPlayerUIWidget::ToggleInfoUI(bool isUse)
{
    if (isUse)
    {
        InfoUI->SetVisibility(ESlateVisibility::Visible);
    }

    else
    {
        InfoUI->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UPlayerUIWidget::UpdateInfoUI(FString name, bool bCanPick)
{
    FString Description = TEXT("F");
    FString Description2 = TEXT("Pick up item");

    FString CombinedText = FString::Printf(TEXT
("%s\n\
[%s] %s"), *name, *Description, *Description2);
    InfoUI->SetText(FText::FromString(CombinedText));
   
}
