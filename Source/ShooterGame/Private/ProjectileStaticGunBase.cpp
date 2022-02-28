// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ProjectileStaticGunBase.h"
#include "GunBase.h"
#include <Components/StaticMeshComponent.h>

AProjectileStaticGunBase::AProjectileStaticGunBase()
{
	BulletType = EBulletType::Projectile;
	MeshType = EMeshType::Static;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;

	Magazine = CreateDefaultSubobject<UStaticMeshComponent>("Magazine");

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Magazine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileStaticGunBase::BeginPlay()
{
	Super::BeginPlay();

	Magazine->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MagazineSocketName);
}