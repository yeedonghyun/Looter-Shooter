#include "MainMenuGameModeBase.h"

bool AMainMenuGameModeBase::bFirstStart = true;

void AMainMenuGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // 마우스 커서 표시
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }

}

void AMainMenuGameModeBase::ShowMainMenu()
{

}
