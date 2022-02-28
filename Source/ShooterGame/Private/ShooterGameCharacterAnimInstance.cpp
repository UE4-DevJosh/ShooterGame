// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ShooterGameCharacterAnimInstance.h"
#include <Math/Vector.h>
#include <GameFramework/Pawn.h>
#include "../ShooterGameCharacter.h"
#include <Animation/AnimInstance.h>
#include <Math/UnrealMathUtility.h>
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "InventoryComponent.h"
#include "GunBase.h"
#include "HealthComponent.h"
#include <Animation/AnimMontage.h>

UShooterGameCharacterAnimInstance::UShooterGameCharacterAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
	AimYaw = 0.0f;
	AimPitch = 0.0f;
	EquippedGunFireRate = 1.0f;
	Aiming = false;
	JumpButtonDown = false;
	FinishedJumping = true;
	Crouching = false;
	Reloading = false;
	Died = false;
	Shooting = false;
	ChangingWeapons = false;
	CanJump = true;
	HoldingWeapon = false;
	IsInAir = false;
}

void UShooterGameCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	AShooterGameCharacter* OwningChar = Cast<AShooterGameCharacter>(TryGetPawnOwner());

	if (OwningChar) {
		Speed = OwningChar->GetSpeed();
		Direction = OwningChar->GetDirection();
		AimYaw = OwningChar->GetAimYaw();
		AimPitch = OwningChar->GetAimPitch();
		EquippedGunFireRate = OwningChar->GetEquippedGunFireRate();
		Aiming = OwningChar->GetWantsToAim();
		Crouching = OwningChar->GetIsCrouching();
		Reloading = OwningChar->GetReloading();
		Died = OwningChar->GetDied();
		Shooting = OwningChar->GetShooting();
		JumpButtonDown = OwningChar->GetJumpButtonDown();
		FinishedJumping = OwningChar->GetFinishedJumping();
		CanJump = OwningChar->GetCanJump();
		IsInAir = OwningChar->GetIsInAir();

		if (InvComp) {
			HoldingWeapon = InvComp->GetHoldingWeapon();
			AGunBase* CurrentWeapon = InvComp->GetCurrentGun();
			if (CurrentWeapon) {
				if (HoldingWeapon) {
					if (Shooting) {
						if (Aiming) {
							UAnimMontage* AimingFireMontage = CurrentWeapon->GetMeshFireMontageAiming();
							if (AimingFireMontage) {
								if (!Montage_IsPlaying(AimingFireMontage)) {
									Montage_Play(AimingFireMontage, EquippedGunFireRate, EMontagePlayReturnType::Duration, 0.0f, false);
								}
							}
						}
						else {
							UAnimMontage* FireMontage = CurrentWeapon->GetMeshFireMontageNonAiming();
							if (FireMontage) {
								if (!Montage_IsPlaying(FireMontage)) {
									Montage_Play(FireMontage, EquippedGunFireRate, EMontagePlayReturnType::Duration, 0.0f, false);
								}
							}
						}
					}
					else {
						UAnimMontage* AimingFireMontage = CurrentWeapon->GetMeshFireMontageAiming();
						UAnimMontage* FireMontage = CurrentWeapon->GetMeshFireMontageNonAiming();
						if (AimingFireMontage && FireMontage) {
							if (Montage_IsPlaying(AimingFireMontage) || Montage_IsPlaying(FireMontage)) {
								Montage_Stop(0.0f, GetCurrentActiveMontage());
							}
						}
					}
				}
			}
			if (Reloading) {
				UAnimMontage* ReloadMontage = CurrentWeapon->GetMeshReloadMontage();
				if (ReloadMontage) {
					if (!Montage_IsPlaying(ReloadMontage)) {
						Montage_Play(ReloadMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, false);
					}
				}
			}
		}
		if (ChangingWeapons) {
			Montage_Stop(0.0f, GetCurrentActiveMontage());
			ChangingWeapons = false;
			Montage_Play(EquipGunMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f, false);
		}
	}
}

void UShooterGameCharacterAnimInstance::OnCurrentGunChanged(AGunBase* NewCurrentGun)
{
	ChangingWeapons = true;
}

void UShooterGameCharacterAnimInstance::SetCanJumpMethod(bool NewJump)
{
	AShooterGameCharacter* OwningChar = Cast<AShooterGameCharacter>(TryGetPawnOwner());

	if (OwningChar) {
	
		OwningChar->SetCanJump(NewJump);
		CanJump = NewJump;
	
	}
}

void UShooterGameCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AShooterGameCharacter* OwningChar = Cast<AShooterGameCharacter>(TryGetPawnOwner());
	if (OwningChar) {
		InvComp = OwningChar->GetInventoryComponent();
	}
}
