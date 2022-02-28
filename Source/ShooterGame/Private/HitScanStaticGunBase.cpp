// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "HitScanStaticGunBase.h"
#include "GunBase.h"
#include <Components/StaticMeshComponent.h>
#include "ShooterGame/ShooterGameCharacter.h"
#include "ShooterGameCharacterAnimInstance.h"
#include <Components/AudioComponent.h>
#include "Sound/SoundCue.h"
#include <DrawDebugHelpers.h>
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <Logging/LogMacros.h>
#include <Engine/Engine.h>
#include "ShooterGame/ShooterGame.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ShooterCharacterPlayerController.h"
#include "ReticleWidget.h"
#include <Net/UnrealNetwork.h>
#include "ShooterGameHUD.h"
#include "InventoryComponent.h"
#include "HUDComponent.h"


AHitScanStaticGunBase::AHitScanStaticGunBase()
{
	//Defaults
	Range = 20000.f;
	BaseDamage = 25.f;
	ReloadTime = 2.17f;
	MagazineSize = 30;
	BulletType = EBulletType::HitScan;
	MeshType = EMeshType::Static;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;

	Magazine = CreateDefaultSubobject<UStaticMeshComponent>("Magazine");

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Magazine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHitScanStaticGunBase::BeginPlay()
{
	Super::BeginPlay();
}

