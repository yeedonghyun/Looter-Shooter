// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "../Inventory/InventorySlot.h"
#include "LooterShooter/Item/ItemBase.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "../Save/SaveManager.h"
#include "Tooltip.h"
#include "Components/Button.h"
#include "../Inventory/Inventory.h"
#include "../Inventory/ItemInventory.h"


#include "InventoryWidgetBase.generated.h"


UENUM(BlueprintType)
enum class EInventoryWidgetType : uint8
{
	PLAY UMETA(DisplayName = "PLAY"),
	STORAGE UMETA(DisplayName = "STORAGE")
};


UCLASS()
class LOOTERSHOOTER_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryWidgetBase")
	EInventoryWidgetType InventoryWidgetType;

	bool bDragging;
	bool bHaveEquipInventory;


	void InitWidget();

	void CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize, EUnderInventoryType InventoryType);


	void SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData);
	void SwapSlotData(UInventorySlot*& DraggingSlot, UInventorySlot*& TargetSlot);

	void HandleSlotActionRequest(UInventorySlot* TargetSlot, ESlotActionType type, bool bActive);
	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot);
	virtual void UseItem(UInventorySlot* TargetSlot);

	UFUNCTION(BlueprintCallable)
	void ShowWarningMessage(FString fs);

	void ToggleWarningMessage(bool bActive);

	UFUNCTION(BlueprintCallable)
	void SaveInventories();

	TArray<UInventorySlot*>& ReturnInventoryArray(EUnderInventoryType InventoryType);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* PlayerInventory;
	TArray<UInventorySlot*> PlayerInventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UItemInventory* EquipInventory;
	TArray<UInventorySlot*> EquipInventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UItemInventory* WorldInventory;
	TArray<UInventorySlot*> WorldBagInventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* WorldBoxInventory;
	TArray<UInventorySlot*> WorldBoxInventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* StorageInventory;
	TArray<UInventorySlot*> StorageInventoryArray;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* SaveButton;

	UTooltip* SlotToolTip;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	//UEditableTextBox* InventoryWarningMessage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* InventoryWarningMessage;

	FTimerHandle UpdateHandle;

};
