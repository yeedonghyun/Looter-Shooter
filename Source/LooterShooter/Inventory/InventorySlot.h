// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "Delegates/DelegateCombinations.h"

#include "LooterShooter/Item/ItemBase.h"

#include "../Item/ItemData.h"

#include "LooterShooter/Item/Item_bag.h"
#include "../Inventory/DragDropSlot.h"

#include "../Inventory/CircleProgressBar.h"


//#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include "InventorySlot.generated.h"


//class UInventoryBase;
//
//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemUseResponse, bool, bSuccess);

UENUM(BlueprintType)
enum class ESlotActionType : uint8
{
	DROP UMETA(DisplayName = "DROP"),
	USE UMETA(DisplayName = "USE"),
	CHECK UMETA(DisplayName = "CHECK"),
	DRAG UMETA(DisplayName = "DRAG")
};


DECLARE_EVENT_TwoParams(UInventorySlot, FSwapSlot, UInventorySlot*, UInventorySlot*)

DECLARE_EVENT_ThreeParams(UInventorySlot, FSlotAction, UInventorySlot*, ESlotActionType, bool)


UENUM(BlueprintType)
enum class EUnderInventoryType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	PLAYER UMETA(DisplayName = "PLAYER"),
	EQUIP UMETA(DisplayName = "EQUIP"),
	WORLDBAG UMETA(DisplayName = "WORLDBAG"),
	WORLDBOX UMETA(DisplayName = "WORLDBOX"),
	STORAGE UMETA(DisplayName = "STORAGE")
};


UCLASS()
class LOOTERSHOOTER_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	EUnderInventoryType UnderInventoryType;

	bool bUseItem;
	float ItemUseDuration;
	float ItemUseDelay;
	//bool bUnderWorldInventory;

	void CreateInventorySlot(int idx, EItemType type, EUnderInventoryType InventoryType);



	void InitInventorySlot(int idx, int InventoryIdx, EItemType type);
	void StartConsume();
	void CompleteConsume();


	void SetSlotFromItem(const FItemData& data);
	void SetSlotFromSlot(const FSlotData& data);
	void ToggleSlot();
	void GetItemImage(FString ItemName);

	void DropItem();

	void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void RequestSlotAction(ESlotActionType type, bool bActive);

	void RequestSwap(UInventorySlot* TargetSlot);



public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
		EItemType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EqiupSlot")
		bool bEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* IMG_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Default;

	UPROPERTY(EditAnywhere, Category = "Slot")
		TSubclassOf<UInventorySlot> DragWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FSlotData SlotData;



	int32 _idx;


	int32 _inventoryIdx; // 체크 필요


	FSlotAction OnSlotActionRequested;

	FSwapSlot OnSwapRequested;


	//FOnItemUseResponse OnUseResponseDelegate;


	//UPROPERTY(meta = (BindWidget))
	UCircleProgressBar* ProgressBar;





	//void RequestUseItem();
	//void OnItemUseResponse(bool bSuccess);

	void TryUseItem();

	//void SetUseRequestDelegate(TFunction<void(UInventorySlot*, FOnSlotActionResult)> InDelegate);

};

