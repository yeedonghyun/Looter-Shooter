// Fill out your copyright notice in the Description page of Project Settings.


#include "Tooltip.h"


void UTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bShouldFollowMouse /*&& IsVisible()*/)
	{
		FVector2D MousePosition;
		if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			SetPositionInViewport(MousePosition + FVector2D(15.0f, 0.0f));
		}
	}
}
