#include "FadeInAndOutWidget.h"

void UFadeInAndOutWidget::PlayFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UFadeInAndOutWidget::PlayFadeOut()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);
	}
}

void UFadeInAndOutWidget::PlayLoading()
{
	if (Loading)
	{
		PlayAnimation(Loading);
	}
}