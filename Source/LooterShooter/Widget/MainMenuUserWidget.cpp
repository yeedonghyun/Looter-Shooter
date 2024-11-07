#include "MainMenuUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Play)
    {
        Play->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnPlayButtonClicked);
        Play->OnHovered.AddDynamic(this, &UMainMenuUserWidget::OnPlayButtonHovered);
        Play->OnUnhovered.AddDynamic(this, &UMainMenuUserWidget::OnPlayButtonUnhovered);
    }

    if (Storage)
    {
        Storage->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnStorageButtonClicked);
        Storage->OnHovered.AddDynamic(this, &UMainMenuUserWidget::OnStorageButtonHovered);
        Storage->OnUnhovered.AddDynamic(this, &UMainMenuUserWidget::OnStorageButtonUnhovered);
    }

    if (Setting)
    {
        Setting->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnSettingButtonClicked);
        Setting->OnHovered.AddDynamic(this, &UMainMenuUserWidget::OnSettingButtonHovered);
        Setting->OnUnhovered.AddDynamic(this, &UMainMenuUserWidget::OnSettingButtonUnhovered);
    }

    if (ExitGame)
    {
        ExitGame->OnClicked.AddDynamic(this, &UMainMenuUserWidget::OnExitGameButtonClicked);
        ExitGame->OnHovered.AddDynamic(this, &UMainMenuUserWidget::OnExitButtonHovered);
        ExitGame->OnUnhovered.AddDynamic(this, &UMainMenuUserWidget::OnExitButtonUnhovered);
    }
}

void UMainMenuUserWidget::OnPlayButtonClicked()
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

void UMainMenuUserWidget::OnStorageButtonClicked()
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

void UMainMenuUserWidget::OnSettingButtonClicked()
{

}

void UMainMenuUserWidget::OnExitGameButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UMainMenuUserWidget::OnPlayButtonHovered()
{

}

void UMainMenuUserWidget::OnPlayButtonUnhovered()
{

}

void UMainMenuUserWidget::OnStorageButtonHovered()
{

}

void UMainMenuUserWidget::OnStorageButtonUnhovered()
{

}

void UMainMenuUserWidget::OnSettingButtonHovered()
{

}

void UMainMenuUserWidget::OnSettingButtonUnhovered()
{

}

void UMainMenuUserWidget::OnExitButtonHovered()
{

}

void UMainMenuUserWidget::OnExitButtonUnhovered()
{

}
