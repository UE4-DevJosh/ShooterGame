// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ShooterPlayerCharacter.h"
#include "../ShooterGameCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include "HUDComponent.h"
#include "ShooterCharacterPlayerController.h"
#include "Interactable.h"
#include <Net/UnrealNetwork.h>
#include "SCMovementComponent.h"
#include <UObject/UObjectGlobals.h>


AShooterPlayerCharacter::AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HUDComponent = CreateDefaultSubobject<UHUDComponent>(TEXT("HUDComponent"));
}

void AShooterPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Target = bWantsToAim ? 80 : 300;
	float NewAim = FMath::FInterpTo(GetCameraBoom()->TargetArmLength, Target, AimInterpSpeed, DeltaTime);
	GetCameraBoom()->TargetArmLength = NewAim;

}

void AShooterPlayerCharacter::SetAbleToPickup(bool bPickup, class AInteractable* NewInteractableItem)
{
	bAbleToPickup = bPickup;
	InteractableItem = NewInteractableItem;
}

void AShooterPlayerCharacter::Pickup()
{
	if (!HasAuthority())
		ServerPickup();

	if (bAbleToPickup && InteractableItem)
		InteractableItem->OnPickup(this);
}

void AShooterPlayerCharacter::ServerPickup_Implementation() {

	Pickup();

}

bool AShooterPlayerCharacter::ServerPickup_Validate()
{
	return true;
}

void AShooterPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterGameCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterGameCharacter::StopJumping);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterGameCharacter::Reload);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AShooterGameCharacter::BeginAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AShooterGameCharacter::EndAim);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterGameCharacter::ToggleCrouch);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShooterGameCharacter::StartFire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AShooterGameCharacter::StopFire);

	PlayerInputComponent->BindAction("Inventory", IE_Released, this, &AShooterPlayerCharacter::ToggleInventory);

	PlayerInputComponent->BindAction("Pickup", IE_Released, this, &AShooterPlayerCharacter::Pickup);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterGameCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterGameCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("MouseWheel", this, &AShooterGameCharacter::MouseWheel);
}


void AShooterPlayerCharacter::ToggleInventory()
{
	if (!HUDComponent)
		return;

	if (bControlsEnabled) {
		HUDComponent->ShowInventory();
	}
	else {
		HUDComponent->HideInventory();
	}
}

void AShooterPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterPlayerCharacter, bAbleToPickup);
	DOREPLIFETIME(AShooterPlayerCharacter, InteractableItem);

}