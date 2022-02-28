// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/TimelineComponent.h>
#include "SCMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API USCMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CCrouch();
	void CUnCrouch();

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCCrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCUnCrouch();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DefaultCapsuleHalfHeight = 96.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CapsuleInterpSpeed = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DefaultMaxWalkSpeed;

	UPROPERTY()
	FVector DefaultCharacterOffset;

	UPROPERTY()
	class UInventoryComponent* OwningInvComp;

	UPROPERTY()
	class USpringArmComponent* OwningCameraBoom;

	UPROPERTY()
	class UCapsuleComponent* OwningCapsuleComp;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* CurveFloat;

	UPROPERTY()
	FTimeline CrouchTimeline;

};
