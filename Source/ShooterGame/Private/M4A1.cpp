// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "M4A1.h"

AM4A1::AM4A1()
{
	GunBoneSocketName = FName("M4A1");
	FireMode = EFireMode::FullAuto;
	WeaponClass = EWeaponClass::Assault;
	FireRate = 0.08f;
	BaseDamage = 20.f;
	Range = 20000;
	MagazineSize = 30.f;
	NumRaycast = 1;
}
