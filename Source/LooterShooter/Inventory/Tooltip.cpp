// Fill out your copyright notice in the Description page of Project Settings.


#include "Tooltip.h"


void UTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D MousePosition;
	if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		SetPositionInViewport(MousePosition + FVector2D(15.0f, 0.0f));
	}
}



void UTooltip::UpdateTexts(FSlotData data, bool bActive)
{
	if (!bActive)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		bShouldFollowMouse = false;
	}

	else
	{
		FVector2D MousePosition;

		if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			SetPositionInViewport(MousePosition + FVector2D(15.0f, 0.0f));
		}

		
	
		Type->SetText(FText::FromString(GetItemTypeAsString_BP(data.Type)));
		Name->SetText(FText::FromString(*data.Name));
		Weight->SetText(FText::AsNumber(data.Weight));
		//Value->SetText(FText::AsNumber(data.Value));
		Value->SetText(FText::Format(FText::FromString("$ {0}"), FText::AsNumber(data.Value)));

		Information->SetText(FText::FromString(*data.Name));

		FString Finfo = "";

		switch (data.Type)
		{
		case EItemType::AMMO :
			Information->SetText(FText::Format(FText::FromString("Add player ammo {0} "), FText::AsNumber(data.Amount)));
			break;

		case EItemType::STUFF:
			break;

		case EItemType::ARMOR:
			Information->SetText(FText::Format(FText::FromString("Add player armor {0} "), FText::AsNumber(data.Value)));
			break;

		case EItemType::HEALING:
			Information->SetText(FText::Format(FText::FromString("Add player health {0} "), FText::AsNumber(data.Value)));
			break;

		case EItemType::INVENTORY:
			Information->SetText(FText::FromString("Equipable bag"));
			break;

		case EItemType::WEAPON:
			Information->SetText(FText::FromString("Equipable weapon"));
			break;

		default:
			break;
		}









		bShouldFollowMouse = true;
		SetVisibility(ESlateVisibility::Visible);
	}
}

FString UTooltip::GetItemTypeAsString_BP(EItemType ItemType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemType"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetDisplayNameTextByValue((int64)ItemType).ToString();
}
