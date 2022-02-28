// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "AK47.h"

AAK47::AAK47()
{
	GunBoneSocketName = FName("AK47");
	FireMode = EFireMode::FullAuto;
	WeaponClass = EWeaponClass::Assault;
	FireRate = 0.1f;
	BaseDamage = 25.f;
	Range = 20000;
	MagazineSize = 30.f;
	NumRaycast = 1;
}