// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterGameCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UShooterGameCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterGameCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void OnCurrentGunChanged(class AGunBase* NewCurrentGun);

	UFUNCTION(BlueprintCallable)
	void SetCanJumpMethod(bool NewJump);

	virtual void NativeBeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float EquippedGunFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool JumpButtonDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool CanJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool FinishedJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool Crouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool Aiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool Shooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool Died;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool ChangingWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool HoldingWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* EquipGunMontage;


	UPROPERTY()
	class UInventoryComponent* InvComp;

};
