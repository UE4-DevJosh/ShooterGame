// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "../ShooterGameCharacter.h"
#include "ShooterPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API AShooterPlayerCharacter : public AShooterGameCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHUDComponent* HUDComponent;

public:
	AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UHUDComponent* GetHUDComponent() { return HUDComponent; };

	UFUNCTION()
	void SetAbleToPickup(bool bPickup, class AInteractable* NewInteractableItem);

	UFUNCTION()
	void Pickup();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickup();

protected:
	void ToggleInventory();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Replicated)
	class AInteractable* InteractableItem;

	UPROPERTY(Replicated)
	bool bAbleToPickup = false;
};
