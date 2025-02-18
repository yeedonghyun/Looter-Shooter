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
    if (Left)    
        Left->SetVisibility(ESlateVisibility::Collapsed);
    
    if (Right)    
        Right->SetVisibility(ESlateVisibility::Collapsed);
    
    if (Top)    
        Top->SetVisibility(ESlateVisibility::Collapsed);
    
    if (Bottom)    
        Bottom->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUIWidget::ShowCrosshairOnAimEnd()
{
    if (Left)
        Left->SetVisibility(ESlateVisibility::Visible);

    if (Right)
        Right->SetVisibility(ESlateVisibility::Visible);

    if (Top)
        Top->SetVisibility(ESlateVisibility::Visible);

    if (Bottom)
        Bottom->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUIWidget::SetLeftAmmoText(int CurAmmo)
{
    FString AmmoString = FString::FromInt(CurAmmo) + TEXT(" / ");
    LeftAmmo->SetText(FText::FromString(AmmoString));
}
