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

#include "../Item/Item_Inventory.h"

#include "Components/TextBlock.h"
#include "../Inventory/Tooltip.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"
#include "../Inventory/CircleProgressBar.h"

#include "PlayerInventoryWidget.generated.h"


DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FDropInventoryItem, FString)
DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FUseInventoryItem, FItemData)
DECLARE_EVENT_OneParam(UPlayerInventoryWidget, FUpdateMagazine, int)

UCLASS()
class LOOTERSHOOTER_API UPlayerInventoryWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool bWorldInventoryOpen;
	bool bUsingItem;


	//AItem_bag* Bag;
	AItem_Inventory* InventoryItem;

	//void InitWidget();
	int FindEmptySlot(TArray<UInventorySlot*>& SlotArray);
	void AddItemEmptySlot(AItemBase* AimedItem);
	void CreateWorldInventory(AItemBase* AimedItem);
	void DeleteWorldInventory();


	virtual void UseItem(UInventorySlot* TargetSlot) override;
	virtual void DropItem(UInventorySlot* TargetSlot) override;
	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot) override;
	virtual void UpdateMagazine() override;

	void ToggleInventory(bool bOpen);
	void SetUIMode(ESlateVisibility Visible, bool showCursor, const FInputModeDataBase& InData, bool bSetIsEnable, float UIOpacity);

	int GetAmmo(int needAmmo);


protected:




public:




	FDropInventoryItem OnDropRequested;
	FUseInventoryItem OnItemUseRequested;
	FUpdateMagazine OnUpdateMagazineRequested;
};
