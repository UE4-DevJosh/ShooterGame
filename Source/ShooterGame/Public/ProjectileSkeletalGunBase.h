// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "ProjectileSkeletalGunBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AProjectileSkeletalGunBase : public AGunBase
{
	GENERATED_BODY()

public:
	AProjectileSkeletalGunBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Magazine;

};

