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
//class LOOTERSHOOTER_API UStorageUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	void InitSlots(UVerticalBox* ParentSlot, TArray<UInventorySlot*>& SlotArray, int32 InventoryIdx, int32 rowSize, int32 colSize);
	void HandleSwapRequest(int32 FromInventorIdx, int32 FromIndex, int32 ToInventoryIdx, int32 ToIndex);
	void ChangeOtherInventoryData(FSlotData& Item, const UInventorySlot& slot);
	UInventorySlot* GetInventorySlot(int32 InventoryIdx, int32 slotIdx);


	UPROPERTY(meta = (BindWidget))
		UVerticalBox* StorageSlots;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* InventorySlots;

	UPROPERTY(EditAnywhere, Category = "Storage")
		int32 RowSize;

	UPROPERTY(EditAnywhere, Category = "Storage")
		int32 ColSize;


	UPROPERTY(meta = (BindWidget))
		UButton* ReturnMain;

	UFUNCTION()
	void OnReturnMainButtonClicked();

	TArray<UInventorySlot*> PlayerInventorySlotArray;
	TArray<UInventorySlot*> StorageSlotArray;
};
