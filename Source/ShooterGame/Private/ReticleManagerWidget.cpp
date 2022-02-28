// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ReticleManagerWidget.h"
#include <Net/UnrealNetwork.h>
#include "../ShooterGameCharacter.h"
#include "InventoryComponent.h"
#include "../Public/GunBase.h"
#include "ReticleWidget.h"

void UReticleManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwningPlayerPawn());

	if (OwningPlayer) {
		UInventoryComponent* InvComp = OwningPlayer->GetInventoryComponent();
		if (InvComp) {
			InvComp->EventChangedEquippedGun.AddUniqueDynamic(this, &UReticleManagerWidget::OnChangeGun);
		}
	}

	if (CurrentReticle) {
		ReticleSize = FVector2D(CurrentReticle->MinimumSize, CurrentReticle->MinimumSize);
	}
	else {
		ReticleSize = FVector2D(35.0f, 35.0f);
	}

}

void UReticleManagerWidget::OnChangeGun(UInventoryComponent* InvComp, AGunBase* NewCurrentGun)
{
	if (NewCurrentGun) {
		CurrentReticle = NewCurrentGun->GetReticle();
		OnReticleChange(NewCurrentGun->GetReticle());
	}
}

void UReticleManagerWidget::TickEvent(float InDeltaTime)
{
	if (!IsInViewport())
		return;

	if (!CurrentReticle)
		return;

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwningPlayerPawn());
	if (OwningPlayer) {
		UInventoryComponent* InvComp = OwningPlayer->GetInventoryComponent();

		if (InvComp) {
			CurrentReticle->SetHit(InvComp->GetHit());
			CurrentReticle->SetReticleSize(InvComp->GetReticleSize());
			CurrentReticle->SetFirstShotAccurate(InvComp->GetFirstShotAccurate());
		}
	}
}