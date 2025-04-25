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
	
		Type->SetText(FText::AsNumber(data.Weight));
		Name->SetText(FText::FromString(*data.Name));
		Weight->SetText(FText::AsNumber(data.Weight));
		Value->SetText(FText::AsNumber(data.Value));
		Information->SetText(FText::FromString(*data.Name));
		bShouldFollowMouse = true;
		SetVisibility(ESlateVisibility::Visible);
	}
}

