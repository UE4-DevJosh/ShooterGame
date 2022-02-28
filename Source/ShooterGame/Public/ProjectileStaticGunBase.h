// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "ProjectileStaticGunBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AProjectileStaticGunBase : public AGunBase
{
	GENERATED_BODY()
public:
	AProjectileStaticGunBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Magazine;
};
