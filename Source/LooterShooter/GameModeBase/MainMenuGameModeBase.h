#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/Userwidget.h"
#include "TimerManager.h"
#include "MainMenuGameModeBase.generated.h"

UCLASS()
class LOOTERSHOOTER_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UUserWidget* MainMenuWidget;

	UPROPERTY()
	UUserWidget* TitleWidget;

	void ShowMainMenu();

	FTimerHandle TimerHandle;

	static bool bFirstStart;
};
