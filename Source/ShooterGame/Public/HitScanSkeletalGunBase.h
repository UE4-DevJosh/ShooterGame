// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "HitScanSkeletalGunBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AHitScanSkeletalGunBase : public AGunBase
{
	GENERATED_BODY()
public:
	AHitScanSkeletalGunBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Magazine;
};
