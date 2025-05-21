#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Inventory/InventorySlot.h"
#include "../Item/ItemData.h"
#include "Components/Button.h"
//#include "../Inventory/InventoryBase.h"
#include "../Inventory/InventoryWidgetBase.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"
#include "../Inventory/Tooltip.h"

#include "StorageUserWidget.generated.h"

UENUM(BlueprintType)
enum class ETradeType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	BUY UMETA(DisplayName = "BUY"),
	SELL UMETA(DisplayName = "SELL"),
};

UCLASS()
class LOOTERSHOOTER_API UStorageUserWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;


	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot) override;


	ETradeType tradeType;


	UPROPERTY(meta = (BindWidget))
		UButton* ReturnMain;

	UFUNCTION(BlueprintCallable)
	void OnReturnMainButtonClicked();

	void MoveItemTradeInventory(UInventorySlot* TargetSlot);

	virtual void HandleSlotRightClickRequest(UInventorySlot* TargetSlot) override;

	void GetBackItem(UInventorySlot* TargetSlot);

	void UpdateMoney();


	UPROPERTY(meta = (BindWidget))
	UButton* Buy;

	UPROPERTY(meta = (BindWidget))
	UButton* Undo;

	UPROPERTY(meta = (BindWidget))
	UButton* Sell;

	UPROPERTY(meta = (BindWidget))
	UButton* Apply;



	UFUNCTION(BlueprintCallable)
	void OnBuyButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnUndoButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnSellButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnApplyButtonClicked();

	int tradingCost;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TradingCost;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Money;


};
