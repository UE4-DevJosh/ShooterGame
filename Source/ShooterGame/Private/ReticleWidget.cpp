// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ReticleWidget.h"
#include <Math/UnrealMathUtility.h>

void UReticleWidget::SetHit(bool bNewHit)
{
	if (bNewHit == bHit)
		return;

	bHit = bNewHit;
	OnChangeHit(bNewHit);
}

void UReticleWidget::SetReticleSize(FVector2D NewReticleSize)
{
	if (NewReticleSize == ReticleSize)
		return;

	ReticleSize = NewReticleSize;
	OnChangeReticleSize(NewReticleSize);
}

void UReticleWidget::SetFirstShotAccurate(bool bNewFirstShotAccurate)
{
	if (bNewFirstShotAccurate == bFirstShotAccurate)
		return;

	bFirstShotAccurate = bNewFirstShotAccurate;
	OnChangeFirstShotAccurate(bNewFirstShotAccurate);
}
