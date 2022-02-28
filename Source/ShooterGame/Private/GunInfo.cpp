// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "GunInfo.h"
#include "../Public/InventoryComponent.h"
#include "../Public/GunBase.h"

void UGunInfo::Setup()
{
	if (InvComp) {

		InvComp->EventLoadedRoundsChanged.AddUniqueDynamic(this, &UGunInfo::EventLoadedRoundsChanged);

		AGunBase* CurrentGun = InvComp->GetCurrentGun();

		if (CurrentGun) {

			EventLoadedRoundsChanged(InvComp, CurrentGun->GetLoadedRounds(), CurrentGun->GetMagazineSize());

		}

	}
}

void UGunInfo::EventLoadedRoundsChanged(UInventoryComponent* InInvComp, int LoadedRounds, int MagazineSize)
{
	OnValueChanged(LoadedRounds, MagazineSize);
}
