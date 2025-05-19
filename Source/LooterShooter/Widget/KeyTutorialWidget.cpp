#include "KeyTutorialWidget.h"

void UKeyTutorialWidget::NativeConstruct()
{
    Super::NativeConstruct();

    KeyInfoMap.Add(Button_MoveFront, FKeyInfo(TEXT("Move Front"), TEXT("The key for moving forward")));
    KeyInfoMap.Add(Button_Retreat, FKeyInfo(TEXT("Retreat"), TEXT("The key for moving backward")));
    KeyInfoMap.Add(Button_MoveLeft, FKeyInfo(TEXT("Move Left"), TEXT("The key for moving left")));
    KeyInfoMap.Add(Button_MoveRight, FKeyInfo(TEXT("Move Right"), TEXT("The key for moving right")));
    KeyInfoMap.Add(Button_Crouch, FKeyInfo(TEXT("Crouch"), TEXT("The key for crouching")));
    KeyInfoMap.Add(Button_LeanLeft, FKeyInfo(TEXT("Lean Left"), TEXT("The key for turning to the left")));
    KeyInfoMap.Add(Button_LeanRight, FKeyInfo(TEXT("Lean Right"), TEXT("The key for turning to the right")));
    KeyInfoMap.Add(Button_Sprint, FKeyInfo(TEXT("Sprint"), TEXT("The key for sprinting")));
    KeyInfoMap.Add(Button_Jump, FKeyInfo(TEXT("Jump"), TEXT("The key for Jumping")));
    KeyInfoMap.Add(Button_InteractToItem, FKeyInfo(TEXT("Interact To Item"), TEXT("The key for interaction to item")));
    KeyInfoMap.Add(Button_ToggleInventory, FKeyInfo(TEXT("Toggle Inventory"), TEXT("The key for toggle inventory")));
    KeyInfoMap.Add(Button_Fire, FKeyInfo(TEXT("Fire"), TEXT("The key for Firing")));
    KeyInfoMap.Add(Button_Aim, FKeyInfo(TEXT("Aim"), TEXT("The key for Aiming")));
    KeyInfoMap.Add(Button_ChangeMagzine, FKeyInfo(TEXT("Change Magzine"), TEXT("The key for changing magazine")));

    for (auto& Elem : KeyInfoMap)
    {
        if (UButton* Button = Elem.Key)
        {
            Button ->OnHovered.AddDynamic(this, &UKeyTutorialWidget::OnAnyHovered);
        }
    }

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = false;
    }

    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(false);
    SetRenderOpacity(0.0f);
}

void UKeyTutorialWidget::OnAnyHovered()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("aaaaa")));

    for (auto& Elem : KeyInfoMap)
    {
        if (Elem.Key->IsHovered())
        {
            HoveredButton = Elem.Key;

            const FKeyInfo& Info = Elem.Value;
            if (KeyNameText) KeyNameText->SetText(FText::FromString(Info.Key));
            if (ExplanationText) ExplanationText->SetText(FText::FromString(Info.Description));
            break;
        }
    }
}

void UKeyTutorialWidget::HideCanvasPanel()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = false;
    }

    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(false);
    SetRenderOpacity(0.0f);
}

void UKeyTutorialWidget::ShowCanvasPanel()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;
    }

    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(true);
    SetRenderOpacity(1.0f);
}