#include "MainMenuGameModeBase.h"

bool AMainMenuGameModeBase::bFirstStart = true;

void AMainMenuGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // ���콺 Ŀ�� ǥ��
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }

    if (TSubclassOf<UUserWidget> MainMenuWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_MainMenuUserWidget.BP_MainMenuUserWidget_C'")))
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }
}