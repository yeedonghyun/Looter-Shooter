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



#include "StorageUserWidget.generated.h"



UCLASS()
class LOOTERSHOOTER_API UStorageUserWidget : public UInventoryBase
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void LoadInventoryData();
	void InitWidget();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventorySlot* EquipInventorySlot;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UInventorySlot* WorldInventorySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventorySlot* ArmorSlot;


	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerInventory;

	//UPROPERTY(meta = (BindWidget))
	//UVerticalBox* WorldInventory;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* EquipInventory;


	UPROPERTY(meta = (BindWidget))
		UButton* ReturnMain;

	UFUNCTION()
	void OnReturnMainButtonClicked();

	TArray<UInventorySlot*> PlayerInventoryArray;
	TArray<UInventorySlot*> EquipInventoryArray;
};
