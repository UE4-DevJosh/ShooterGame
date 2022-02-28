// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ProjectileSkeletalGunBase.h"
#include "GunBase.h"
#include <Components/SkeletalMeshComponent.h>

AProjectileSkeletalGunBase::AProjectileSkeletalGunBase()
{
	BulletType = EBulletType::Projectile;
	MeshType = EMeshType::Skeletal;

	Body = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	RootComponent = Body;

	Magazine = CreateDefaultSubobject<USkeletalMeshComponent>("Magazine");

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Magazine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileSkeletalGunBase::BeginPlay()
{
	Super::BeginPlay();

	Magazine->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MagazineSocketName);

}