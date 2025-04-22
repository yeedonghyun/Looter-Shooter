// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "../Inventory/InventorySlot.h"
#include "../Inventory/InventoryBase.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Item/Item_bag.h"
#include "Components/TextBlock.h"
#include "../Inventory/Tooltip.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"


#include "PlayerInventoryWidget.generated.h"


DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FDropInventoryItem, FString)
DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FUseInventoryItem, FItemData)

UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UInventoryBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void InitWidget();
	void LoadInventoryData();


	int FindEmptySlot(TArray<UInventorySlot*>& SlotArray);
	void AddItemEmptySlot(AItemBase* AimedItem);
	void CreateWorldInventory(AItemBase* AimedItem);
	void DeleteWorldInventory();

	void ToggleInventory(bool bOpen);
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData);


	virtual void UseItem(FItemData data) override;
	virtual void HandleSwapRequest(UInventorySlot* From, UInventorySlot* To) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UItemInventory* EquipInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UItemInventory* WorldInventory;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInventorySlot* ArmorSlot;

	


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



	AItem_bag* Bag;

	FDropInventoryItem OnDropRequested;
	FUseInventoryItem OnItemUseRequested;
};
