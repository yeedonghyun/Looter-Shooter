// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"

#include "PlayerInventoryWidget.generated.h"


UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void OpenInventory();
	void CloseInventory();
	void AddItem(AItemBase& item);
	virtual void NativeConstruct() override;
	void HandleSwapRequest(int32 FromIndex, int32 ToIndex);

protected:

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* PlayerSlot;

	TArray<UInventorySlot*> Slots;

public:

};
