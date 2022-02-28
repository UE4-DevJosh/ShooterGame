// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "GunBase.h"
#include <Components/AudioComponent.h>
#include "Sound/SoundCue.h"
#include <GameFramework/Actor.h>
#include <Sound/SoundAttenuation.h>
#include "../ShooterGameCharacter.h"
#include "InventoryComponent.h"
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Kismet/GameplayStatics.h>
#include "../ShooterGame.h"
#include "ShooterCharacterPlayerController.h"
#include <Particles/ParticleSystemComponent.h>
#include <Net/UnrealNetwork.h>
#include <Components/SceneComponent.h>
#include "ReticleWidget.h"
#include "ShooterPlayerCharacter.h"
#include <Animation/AnimMontage.h>
#include <DrawDebugHelpers.h>
#include <Engine/Texture2D.h>

AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//Allowing the Gun Base class to replicate
	SetReplicates(true);
	SetReplicateMovement(true);

	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 33.0f;

}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerChar = Cast<AShooterGameCharacter>(GetOwner());

	if (OwnerChar)
		OwningInvComp = OwnerChar->GetInventoryComponent();

	if (GetMagazineStatic()) {
		GetMagazineStatic()->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetMagazineSocketName());
	}
	else if (GetMagazineSkeletal()) {
		GetMagazineSkeletal()->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetMagazineSocketName());
	}

	FireRate = GetFireRate();

	LoadedRounds = GetMagazineSize();

	AShooterPlayerCharacter* OwningCharacter = Cast<AShooterPlayerCharacter>(GetOwner());

	if (OwningCharacter) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningCharacter->GetController());
		if (OwningController) {
			if (OwningController->IsLocalController()) {
				GunReticle = CreateWidget<UReticleWidget>(OwningController, ReticleWidgetSubclass);
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// GUN MECHANICS
///

void AGunBase::StartFire()
{
	if (GetFireMode() == FullAuto) {
		float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AGunBase::Fire, FireRate, true, FirstDelay);
	}
	else {
		float TimeBetweenShots = FMath::Max(LastFireTime + FireRate - GetWorld()->TimeSeconds, 0.0f);
		if (TimeBetweenShots > 0.0f) {
			return;
		}
		else {
			Fire();
		}
	}
}

void AGunBase::Fire()
{
	if (!(GetBodyStatic() || GetBodySkeletal()))
		return;

	int NewLoadedRounds = GetLoadedRounds() - 1;

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	if (NewLoadedRounds < 0) {

		if (OwningPlayer) {

			OwningPlayer->StopFire();
			OwningPlayer->SetOutOfAmmo(true);

		}

		return;
	}

	if (OwningInvComp)
		OwningInvComp->TriggerFireEvent();

	LoadedRounds = NewLoadedRounds;

	UInventoryComponent* InvComp = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));

	if (InvComp)
		InvComp->EventLoadedRoundsChanged.Broadcast(InvComp, GetLoadedRounds(), GetMagazineSize());

	if (GetBulletType() == HitScan) {

		if (OwningPlayer) {

			AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());

			FVector EyeLocation;
			FRotator EyeRotation;
			FQuat WeaponRotation;
			FVector StartTrace;

			OwningPlayer->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			if (GetBodyStatic()) {
				GetBodyStatic()->GetSocketWorldLocationAndRotation(GetMuzzleSocketName(), StartTrace, WeaponRotation);
			}
			else if (GetBodySkeletal()) {
				GetBodySkeletal()->GetSocketWorldLocationAndRotation(GetMuzzleSocketName(), StartTrace, WeaponRotation);
			}
			else {
				return;
			}

			FVector ShotDirection = EyeRotation.Vector();

			for (int i = 0; i < NumRaycast; i++)
			{
				if (InvComp) {
					if (!InvComp->GetFirstShotAccurate() || i > 0) {
						float BulletBloom = InvComp->GetReticleSize().X / BulletBloomDivisor;
						float HalfRad = FMath::DegreesToRadians(BulletBloom);
						ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
					}
				}

				FVector EndTrace = ((ShotDirection * GetRange()) + EyeLocation);
				FVector InitialStartTrace = EyeLocation;
				FVector InitialEndTrace = EndTrace;

				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(OwningPlayer);
				QueryParams.AddIgnoredActor(this);
				QueryParams.bTraceComplex = true;
				QueryParams.bReturnPhysicalMaterial = true;

				FHitResult HitResult;
				FHitResult InitialHitResult;

				if (GetWorld()->LineTraceSingleByChannel(InitialHitResult, InitialStartTrace, InitialEndTrace, COLLISION_WEAPONTRACE, QueryParams)) {
					EndTrace = InitialHitResult.Location;
				}

				EPhysicalSurface SurfaceType = SurfaceType_Default;

				if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, COLLISION_WEAPONTRACE, QueryParams)) {
					EndTrace = HitResult.Location;
					AActor* HitActor = HitResult.GetActor();
					SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
					float ActualDamage = ((SurfaceType == SURFACE_FLESHVULNERABLE) ? GetBaseDamage() * 4.f : GetBaseDamage()) / NumRaycast;
					UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, HitResult, OwningPlayer->GetInstigatorController(), this, DamageType);
					PlayImpactEffects(SurfaceType, OwningController, InvComp);
				}

				if (HasAuthority()) {
					HitScanTrace.EndTrace = EndTrace;
					HitScanTrace.OwningController = OwningController;
					HitScanTrace.SurfaceType = SurfaceType;
					if (i == 0) {
						PlayMulticastSound();
					}
					PlayMulticastEffects(EndTrace);
				}

			}
			LastFireTime = GetWorld()->TimeSeconds;
		}
	}
	else if (GetBulletType() == Projectile) {

		//PROJECTILE WEAPONS

	}
}

void AGunBase::PlayImpactEffects(EPhysicalSurface SurfaceType, AShooterCharacterPlayerController* OwningController, UInventoryComponent* InInvComp)
{
	bool hitFlesh = false;

	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
		hitFlesh = true;
		break;
	case SURFACE_FLESHVULNERABLE:
		hitFlesh = true;
		break;
	default:
		break;
	}

	if (hitFlesh && OwningController) {
		if (OwningController->IsLocalController()) {
			if (InInvComp) {
				InInvComp->SetHit();
			}
		}
	}
}

void AGunBase::PlayEffects(FVector TracerEndPoint)
{
	FVector MuzzleLocation = FVector::ZeroVector;
	
	if (GetBodyStatic()) {
		MuzzleLocation = GetBodyStatic()->GetSocketLocation(GetMuzzleSocketName());
	}
	else if (GetBodySkeletal()) {
		MuzzleLocation = GetBodySkeletal()->GetSocketLocation(GetMuzzleSocketName());
	}

	if (MuzzleEffect) {
		int32 RandomChance = FMath::RandRange(0, 100);
		if (RandomChance <= 15) {

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, MuzzleLocation);

		}
	}

	if (TracerEffect) {
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp) {
			TracerComp->SetBeamTargetPoint(0, TracerEndPoint, 0);
		}
	}
}

void AGunBase::PlayMulticastEffects_Implementation(FVector TracerEndPoint)
{
	PlayEffects(TracerEndPoint);
}

void AGunBase::PlayMulticastSound_Implementation()
{

	if (GunShotSoundCue) {

		if (GetMagazineStatic()) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunShotSoundCue, GetBodyStatic()->GetSocketLocation(GetMuzzleSocketName()), 1.0f, 1.0f, 0.0f, GunShotSoundAttenuation);
		}
		else if (GetMagazineSkeletal()) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunShotSoundCue, GetBodySkeletal()->GetSocketLocation(GetMuzzleSocketName()), 1.0f, 1.0f, 0.0f, GunShotSoundAttenuation);
		}

	}
}
void AGunBase::OnRep_HitScanTrace()
{
	PlayEffects(HitScanTrace.EndTrace);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.OwningController, nullptr);
}

void AGunBase::Reload()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &AGunBase::EndReload, GetReloadTime(), false);
}

void AGunBase::EndReload()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Reload);

	LoadedRounds = GetMagazineSize();

	UInventoryComponent* InvComp = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));

	if (InvComp)
		InvComp->EventLoadedRoundsChanged.Broadcast(InvComp, GetLoadedRounds(), GetMagazineSize());

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	if (OwningPlayer)
		OwningPlayer->SetOutOfAmmo(false);
}

void AGunBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
}

void AGunBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGunBase, HitScanTrace, COND_SkipOwner);

}
