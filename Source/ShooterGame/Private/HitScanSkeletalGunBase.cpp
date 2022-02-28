// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "HitScanSkeletalGunBase.h"
#include "GunBase.h"
#include <Components/SkeletalMeshComponent.h>

AHitScanSkeletalGunBase::AHitScanSkeletalGunBase()
{
	BulletType = EBulletType::HitScan;
	MeshType = EMeshType::Skeletal;

	Body = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	RootComponent = Body;

	Magazine = CreateDefaultSubobject<USkeletalMeshComponent>("Magazine");

	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Magazine->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AHitScanSkeletalGunBase::BeginPlay()
{
	Super::BeginPlay();

	Magazine->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MuzzleSocketName);
}