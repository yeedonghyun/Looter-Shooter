// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"

#include "PlayerInventoryWidget.generated.h"


UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void OpenInventory();
	void CloseInventory();

	virtual void NativeConstruct() override;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;

protected:


	UPROPERTY(meta = (BindWidget))
		UUniformGridPanel* InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Slot")
		TSubclassOf<UInventorySlot> DragWidgetClass;
	
	TArray<UInventorySlot*> Slots;

public:

};
