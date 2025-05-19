#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "KeyTutorialWidget.generated.h"

USTRUCT()
struct FKeyInfo
{
    GENERATED_BODY()

    FString Key;
    FString Description;

    FKeyInfo() {}
    FKeyInfo(const FString& InKey, const FString& InDesc)
        : Key(InKey), Description(InDesc) {
    }
};

UCLASS()
class LOOTERSHOOTER_API UKeyTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* CanvasPanel;

    UFUNCTION(BlueprintCallable)
    void HideCanvasPanel();

    UFUNCTION(BlueprintCallable)
    void ShowCanvasPanel();

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget)) UTextBlock* KeyNameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ExplanationText;

    UPROPERTY(meta = (BindWidget)) UButton* Button_MoveFront;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Retreat;
    UPROPERTY(meta = (BindWidget)) UButton* Button_MoveLeft;
    UPROPERTY(meta = (BindWidget)) UButton* Button_MoveRight;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Crouch;
    UPROPERTY(meta = (BindWidget)) UButton* Button_LeanLeft;
    UPROPERTY(meta = (BindWidget)) UButton* Button_LeanRight;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Sprint;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Jump;
    UPROPERTY(meta = (BindWidget)) UButton* Button_InteractToItem;
    UPROPERTY(meta = (BindWidget)) UButton* Button_ToggleInventory;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Fire;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Aim;
    UPROPERTY(meta = (BindWidget)) UButton* Button_ChangeMagzine;

    UPROPERTY()
    UButton* HoveredButton = nullptr;
    TMap<UButton*, FKeyInfo> KeyInfoMap;

    UFUNCTION()
    void OnAnyHovered();
};
