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
#include "../Inventory/RightClickOption.h"
#include "Components/ProgressBar.h"
#include <LooterShooter/Item/Item_Inventory.h>

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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryWidgetBase")
	EInventoryWidgetType InventoryWidgetType;

	bool bDragging;
	bool bHaveEquipInventory;
	bool bOpenRightClickOption;
	bool bClickDetectionEnabled;

	void InitWidget();

	void CreateInventory(TArray<UInventorySlot*>& SlotArray, UVerticalBox* ParentGrid, int32 rowSize, int32 colSize, EUnderInventoryType InventoryType);


	void SetArrayData(TArray<UInventorySlot*>& SlotArray, TArray<FSlotData> ArrayData);
	void SwapSlotData(UInventorySlot*& DraggingSlot, UInventorySlot*& TargetSlot);

	void HandleSlotActionRequest(UInventorySlot* TargetSlot, ESlotActionType type, bool bActive);
	virtual void HandleSwapRequest(UInventorySlot* DraggingSlot, UInventorySlot* TargetSlot);

	virtual void HandleSlotRightClickRequest(UInventorySlot* TargetSlot);

	virtual void UseItem(UInventorySlot* TargetSlot);
	virtual void DropItem(UInventorySlot* TargetSlot);
	virtual void UpdateMagazine();

	UFUNCTION(BlueprintCallable)
	void ShowWarningMessage(FString fs);

	void ToggleWarningMessage(bool bActive);

	UFUNCTION(BlueprintCallable)
	void SaveInventories();

	TArray<UInventorySlot*>& ReturnInventoryArray(EUnderInventoryType InventoryType);

	UFUNCTION(BlueprintCallable)
	void OnSlotActionSelected(ESlotActionType ActionType);


	void CreateClickOption();


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	//Inventorys-----------------------------------------------------------------//

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* ShopInventory;
	TArray<UInventorySlot*> ShopInventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventory* TradingInventory;
	TArray<UInventorySlot*> TradingInventoryArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventorySlot* WeaponSlot;



	UTooltip* SlotToolTip;


	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* InventoryWarningMessage;

	URightClickOption* RightClickOption;


	FTimerHandle UpdateHandle;


	UInventorySlot* DedicateSlot;







	void CheckAmmo();
	void UpdateAmmo();

	int sumAmmo;

	UInventorySlot* FirstAmmoSlot;



	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* AmmoCnt;

	int32 money;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UProgressBar* UHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional))
	UProgressBar* UArmor;


	UFUNCTION(BlueprintCallable)
	void SetHealth(float TargetValue);
	UFUNCTION(BlueprintCallable)
	void SetArmor(float TargetValue);
	UFUNCTION(BlueprintCallable)
	void UpdateHealth();
	UFUNCTION(BlueprintCallable)
	void UpdateArmor();

	FTimerHandle HealthUpdateTimerHandle;
	FTimerHandle ArmorUpdateTimerHandle;

	float HealthTargetValue;
	float ArmorTargetValue;
};
