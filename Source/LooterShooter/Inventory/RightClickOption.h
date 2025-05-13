// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Delegates/DelegateCombinations.h"
#include "../Inventory/InventorySlot.h"


#include "RightClickOption.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotActionSelected, ESlotActionType, ActionType);

UCLASS()
class LOOTERSHOOTER_API URightClickOption : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BUse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BDrop;

	UFUNCTION()
	void OnUseClicked();

	UFUNCTION()
	void OnDropClicked();




	FOnSlotActionSelected OnActionSelected;
};
