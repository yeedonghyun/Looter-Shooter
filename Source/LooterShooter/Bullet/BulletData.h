// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BulletData.generated.h"



UCLASS()
class LOOTERSHOOTER_API UBulletData : public UDataTable
{
	GENERATED_BODY()

public:
	FString Name;
	int Value;
	int maxAmount;
	
};
