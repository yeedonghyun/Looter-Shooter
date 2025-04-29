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



UCLASS()
class LOOTERSHOOTER_API UStorageUserWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;


	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot) override;



	UPROPERTY(meta = (BindWidget))
		UButton* ReturnMain;

	UFUNCTION(BlueprintCallable)
	void OnReturnMainButtonClicked();



};
