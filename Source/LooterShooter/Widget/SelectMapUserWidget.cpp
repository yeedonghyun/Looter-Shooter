#include "SelectMapUserWidget.h"
#include "Kismet/GameplayStatics.h"

void USelectMapUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Play)
    {
        Play->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonClicked);
        Play->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonHovered);
        Play->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonUnhovered);
    }

    if (Storage)
    {
        Storage->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnStorageButtonClicked);
        Storage->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnStorageButtonHovered);
        Storage->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnStorageButtonUnhovered);
    }

    if (Setting)
    {
        Setting->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnSettingButtonClicked);
        Setting->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnSettingButtonHovered);
        Setting->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnSettingButtonUnhovered);
    }

    if (ExitGame)
    {
        ExitGame->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnExitGameButtonClicked);
        ExitGame->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnExitButtonHovered);
        ExitGame->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnExitButtonUnhovered);
    }
}

void USelectMapUserWidget::OnPlayButtonClicked()
{
    this->RemoveFromParent();

    if (TSubclassOf<UUserWidget> SelectMapWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_SelectMapUserWidget.BP_SelectMapUserWidget_C'")))
    {
        UUserWidget* SelectMapWidget = CreateWidget<UUserWidget>(GetWorld(), SelectMapWidgetClass);
        if (SelectMapWidget)
        {
            SelectMapWidget->AddToViewport();
        }
    }
}

void USelectMapUserWidget::OnStorageButtonClicked()
{
    this->RemoveFromParent();

    if (TSubclassOf<UUserWidget> StorageWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_StorageUserWidget.BP_StorageUserWidget_C'")))
    {
        UUserWidget* StorageWidget = CreateWidget<UUserWidget>(GetWorld(), StorageWidgetClass);
        if (StorageWidget)
        {
            StorageWidget->AddToViewport();
        }
    }
}

void USelectMapUserWidget::OnSettingButtonClicked()
{

}

void USelectMapUserWidget::OnExitGameButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void USelectMapUserWidget::OnPlayButtonHovered()
{

}

void USelectMapUserWidget::OnPlayButtonUnhovered()
{

}

void USelectMapUserWidget::OnStorageButtonHovered()
{

}

void USelectMapUserWidget::OnStorageButtonUnhovered()
{

}

void USelectMapUserWidget::OnSettingButtonHovered()
{

}

void USelectMapUserWidget::OnSettingButtonUnhovered()
{

}

void USelectMapUserWidget::OnExitButtonHovered()
{

}

void USelectMapUserWidget::OnExitButtonUnhovered()
{

}
