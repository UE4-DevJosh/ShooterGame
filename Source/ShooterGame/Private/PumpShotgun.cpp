// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "PumpShotgun.h"

APumpShotgun::APumpShotgun()
{
	GunBoneSocketName = FName("PumpShotgun");
	FireMode = EFireMode::FullAuto;
	WeaponClass = EWeaponClass::Shotgun;
	FireRate = 0.75f;
	BaseDamage = 100.0f;
	Range = 1000;
	MagazineSize = 10.f;
	NumRaycast = 6;
}
