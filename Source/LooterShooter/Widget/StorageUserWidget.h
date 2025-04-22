#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Inventory/InventorySlot.h"
#include "../Item/ItemData.h"
#include "Components/Button.h"
#include "../Inventory/InventoryBase.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"
#include "../Inventory/Tooltip.h"

#include "StorageUserWidget.generated.h"



UCLASS()
class LOOTERSHOOTER_API UStorageUserWidget : public UInventoryBase
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void LoadInventoryData();
	void InitWidget();

	virtual void UseItem(FItemData data) override;
	virtual void HandleSwapRequest(UInventorySlot* From, UInventorySlot* To) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UItemInventory* EquipInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventory* StorageInventory;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventorySlot* ArmorSlot;


	UPROPERTY(meta = (BindWidget))
		UButton* ReturnMain;

	UFUNCTION(BlueprintCallable)
	void OnReturnMainButtonClicked();
	UFUNCTION(BlueprintCallable)
	void OnSaveButtonClicked();


	TArray<UInventorySlot*> PlayerInventoryArray;
	TArray<UInventorySlot*> EquipInventoryArray;
	TArray<UInventorySlot*> StorageInventoryArray;
};
