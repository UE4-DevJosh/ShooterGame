// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "HitScanStaticGunBase.generated.h"

UCLASS()
class SHOOTERGAME_API AHitScanStaticGunBase : public AGunBase
{
	GENERATED_BODY()

public:
	AHitScanStaticGunBase();
	
	virtual class UStaticMeshComponent* GetBodyStatic() override { return Body; };
	virtual class UStaticMeshComponent* GetMagazineStatic() override { return Magazine; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Magazine;

	
};
