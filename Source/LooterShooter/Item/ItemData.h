// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	WEAPON UMETA(DisplayName = "Weapon"),
	AMMO UMETA(DisplayName = "Ammo"),
	HEALING UMETA(DisplayName = "Healing"),
	BAG UMETA(DisplayName = "Bag")
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData()
	{
		// �ʱⰪ ����
		Name = TEXT("");   // �� ���ڿ�
		Value = 0;         // �⺻�� 0
		Weight = 0;        // �⺻�� 0
		Type = EItemType::WEAPON;  // �⺻��: WEAPON
	}

	void SetItemFromSlotData(FSlotData data);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		EItemType Type;
};

USTRUCT(BlueprintType)
struct FSlotData : public FItemData
{
	GENERATED_BODY()

public:
	FSlotData()
	{
		// FSlotData �⺻�� ����
		bHaveItem = false; // ���Կ� �������� ���� �⺻��
	}
	void SetSlotFromItemData(FItemData data);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		bool bHaveItem;
};




class LOOTERSHOOTER_API ItemData
{
public:
	ItemData();
	~ItemData();
};