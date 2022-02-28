// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventStartAim, AShooterGameCharacter*, ShooterGameChar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventStopAim, AShooterGameCharacter*, ShooterGameChar);

UCLASS(config=Game)
class AShooterGameCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

public:
	AShooterGameCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void CalcAnimInstanceValues(float DeltaSeconds);
	float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UHealthComponent* GetHealthComponent() { return HealthComponent; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UInventoryComponent* GetInventoryComponent() { return InventoryComponent; };
	
	//BOOL GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsCrouching() { return bCrouching; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetWantsToAim() { return bWantsToAim; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetReloading() { return bReloading; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetShooting() { return bShooting; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetJumpButtonDown() { return bJumpButtonDown; };

	UFUNCTION(BlueprintCallable)
	void SetJumpButtonDown(bool NewJump) { bJumpButtonDown = NewJump; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCanJump() { return bCanJump; };

	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool NewJump) { bCanJump = NewJump; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetFinishedJumping() { return bFinishedJumping; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetControlsEnabled() { return bControlsEnabled; };

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void BeginAim();

	UFUNCTION(BlueprintCallable)
	void EndAim();

	UFUNCTION(BlueprintCallable)
	void SetControlsEnabled(bool NewControlsEnabled) { bControlsEnabled = NewControlsEnabled; };

	void SetCanFire(bool NewFire) { bCanFire = NewFire; };
	void SetOutOfAmmo(bool NewOutOfAmmo) { bOutOfAmmo = NewOutOfAmmo; };

	float GetSpeed() { return Speed; };
	float GetDirection() { return Direction; };
	float GetAimYaw() { return AimYaw; };
	float GetAimPitch() { return AimPitch; };
	float GetEquippedGunFireRate() { return EquippedGunFireRate; };
	bool GetDied() { return bDied; };
	bool GetIsInAir() { return bIsInAir; };

	virtual void Jump() override;
	virtual void StopJumping() override;
	void Reload();
	void FinishedReloading();
	void ToggleCrouch();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void MouseWheel(float Value);
	void OnCurrentGunChanged(class AGunBase* NewCurrentGun);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventStartAim EventStartAim;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventStopAim EventStopAim;

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJump();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopJumping();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFinishedReloading();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleCrouch();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginAim();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndAim();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTurnAtRate(float Rate);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerLookUpAtRate(float Rate);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMouseWheel(float Value);

	UPROPERTY()
	FTimerHandle TimerHandle_Reloading;

	UPROPERTY()
	float BaseTurnRate;
	UPROPERTY()
	float BaseLookUpRate;
	UPROPERTY()
	float AimInterpSpeed = 10.0f;

	UPROPERTY(Replicated)
	bool bControlsEnabled = true;

	UPROPERTY(Replicated)
	bool bCrouching = false;

	UPROPERTY(Replicated)
	bool bShooting = false;

	UPROPERTY(Replicated)
	bool bJumpButtonDown = false;

	UPROPERTY(Replicated)
	bool bReloading = false;

	UPROPERTY(Replicated)
	bool bWantsToAim = false;

	/// PROPERTIES FROM ANIM INSTANCE =--------------------------------=------------------------------=---------------

	UPROPERTY(Replicated)
	bool bDied = false;

	UPROPERTY(Replicated)
	bool bCanJump = true;

	UPROPERTY(Replicated)
	bool bOutOfAmmo = false;

	UPROPERTY(Replicated)
	bool bFinishedJumping = true;

	UPROPERTY(Replicated)
	bool bCanFire = true;

	UPROPERTY(Replicated)
	float Speed = 0.0f;

	UPROPERTY(Replicated)
	float Direction = 0.0f;

	UPROPERTY(Replicated)
	float AimYaw = 0.0f;

	UPROPERTY(Replicated)
	float AimPitch = 0.0f;

	UPROPERTY(Replicated)
	float EquippedGunFireRate = 0.0f;

	UPROPERTY(Replicated)
	bool bIsInAir = false;

	UPROPERTY(Replicated)
	float DeltaSecs = 0.0f;

	/// =--------------------------------=------------------------------=---------------

public:
	virtual FVector GetPawnViewLocation() const override;
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

