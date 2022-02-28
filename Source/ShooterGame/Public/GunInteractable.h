// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GunInteractable.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AGunInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	AGunInteractable();

	virtual void BeginPlay() override;
	virtual void OnPickup(class AActor* PickupInstigator) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnPickupMulticast(class AActor* PickupInstigator);

protected:
	UFUNCTION()
	void OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGunBase> PickupGun;
};
