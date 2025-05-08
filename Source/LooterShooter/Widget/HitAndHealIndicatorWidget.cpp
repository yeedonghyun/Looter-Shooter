#include "HitAndHealIndicatorWidget.h"

void UHitAndHealIndicatorWidget::PlayHit()
{
	if (Hit)
	{
		PlayAnimation(Hit);
	}
}

void UHitAndHealIndicatorWidget::PlayHealHP()
{
	if (HealHP)
	{
		PlayAnimation(HealHP);
	}
}

void UHitAndHealIndicatorWidget::PlayHealArmor()
{
	if (HealArmor)
	{
		PlayAnimation(HealArmor);
	}
}