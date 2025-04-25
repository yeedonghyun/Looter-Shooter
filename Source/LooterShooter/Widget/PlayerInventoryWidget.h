// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"

//#include "../Inventory/InventoryBase.h"
#include "../Inventory/InventoryWidgetBase.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Item/Item_bag.h"
#include "Components/TextBlock.h"
#include "../Inventory/Tooltip.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"
#include "../Inventory/CircleProgressBar.h"

#include "PlayerInventoryWidget.generated.h"


DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FDropInventoryItem, FString)
DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FUseInventoryItem, FItemData)

UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool bWorldInventoryOpen;
	bool bUsingItem;

	AItem_bag* Bag;

	//	UTooltip* SlotToolTip;
	//virtual void Tick(float DeltaTime) override;

	void InitWidget();
	int FindEmptySlot(TArray<UInventorySlot*>& SlotArray);
	void AddItemEmptySlot(AItemBase* AimedItem);
	void CreateWorldInventory(AItemBase* AimedItem);
	void DeleteWorldInventory();


	void ToggleInventory(bool bOpen);
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData);


	virtual void UseItem(UInventorySlot* TargetSlot) override;
	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UItemInventory* EquipInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UItemInventory* WorldInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UItemInventory* WorldBoxInventory;






	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UInventorySlot* ArmorSlot;



public:

	TArray<UInventorySlot*> PlayerInventoryArray;
	TArray<UInventorySlot*> WorldInventoryArray;
	TArray<UInventorySlot*> EquipInventoryArray;

	int32 InventoryRowSize;
	int32 InventoryColSize;

	UFUNCTION(BlueprintCallable)
	void OnSaveButtonClicked();


	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bEquipInventory;




	bool bOtherInventory;




	FDropInventoryItem OnDropRequested;
	FUseInventoryItem OnItemUseRequested;
};
