// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "SCMovementComponent.h"
#include <UObject/UObjectGlobals.h>
#include <Net/UnrealNetwork.h>
#include "../ShooterGameCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "InventoryComponent.h"
#include <Curves/CurveFloat.h>
#include <Math/UnrealMathUtility.h>
#include <GameFramework/Controller.h>
#include "ShooterCharacterPlayerController.h"

USCMovementComponent::USCMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	AShooterGameCharacter* OwningChar = Cast<AShooterGameCharacter>(CharacterOwner);

	if (OwningChar) {
		UInventoryComponent* OIC = OwningChar->GetInventoryComponent();
		if (OIC) {
			OwningInvComp = OIC;
		}
		UCapsuleComponent* OCC = OwningChar->GetCapsuleComponent();
		if (OCC) {
			OwningCapsuleComp = OCC;
			DefaultCapsuleHalfHeight = OwningCapsuleComp->GetScaledCapsuleHalfHeight();
		}
		USpringArmComponent* OSAC = OwningChar->GetCameraBoom();
		if (OSAC) {
			OwningCameraBoom = OSAC;
		}

		DefaultCharacterOffset = OwningChar->GetMesh()->GetRelativeLocation();
	}

	DefaultMaxWalkSpeed = MaxWalkSpeed;

	if (CurveFloat) {
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CrouchTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

void USCMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwningInvComp)
		return;

	CrouchTimeline.TickTimeline(DeltaTime);
}

void USCMovementComponent::CCrouch()
{
	AShooterCharacterPlayerController* OwningCont = Cast<AShooterCharacterPlayerController>(CharacterOwner->GetController());

	if (OwningCont) {
		if (OwningCont->IsLocalPlayerController()) {
			CrouchTimeline.Play();
			MulticastCCrouch();
		}
	}

}

void USCMovementComponent::CUnCrouch()
{
	AShooterCharacterPlayerController* OwningCont = Cast<AShooterCharacterPlayerController>(CharacterOwner->GetController());

	if (OwningCont) {
		if (OwningCont->IsLocalPlayerController()) {
			CrouchTimeline.Reverse();
			MulticastCUnCrouch();
		}
	}
}

void USCMovementComponent::TimelineProgress(float Value)
{
	OwningCapsuleComp->SetCapsuleHalfHeight(FMath::Lerp(DefaultCapsuleHalfHeight, CrouchedHalfHeight, Value));
	MaxWalkSpeed = (FMath::Lerp(DefaultMaxWalkSpeed, MaxWalkSpeedCrouched, Value));
	FVector NewCharOffset = DefaultCharacterOffset;
	NewCharOffset.Z += DefaultCapsuleHalfHeight / 2;
	CharacterOwner->GetMesh()->SetRelativeLocation(FMath::Lerp(DefaultCharacterOffset, NewCharOffset, Value));
}

void USCMovementComponent::MulticastCCrouch_Implementation()
{
	Crouch(true);
}

void USCMovementComponent::MulticastCUnCrouch_Implementation()
{
	UnCrouch(true);
}