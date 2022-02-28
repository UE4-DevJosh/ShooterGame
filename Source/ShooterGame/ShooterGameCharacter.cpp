// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#include "ShooterGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InventoryComponent.h"
#include <GameFramework/Character.h>
#include "GunBase.h"
#include <GameFramework/DamageType.h>
#include "HealthComponent.h"
#include <GameFramework/Actor.h>
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimationAsset.h>
#include <GameFramework/NavMovementComponent.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h> 
#include "ShooterCharacterPlayerController.h"
#include "InventoryWidget.h"
#include "ShooterGameCharacterAnimInstance.h"
#include "ShooterGameHUD.h"
#include "HUDComponent.h"
#include <CollisionShape.h>
#include "SCMovementComponent.h"
#include <UObject/UObjectGlobals.h>

AShooterGameCharacter::AShooterGameCharacter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USCMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 33.0f;

}

void AShooterGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaSecs = DeltaTime;

	CalcAnimInstanceValues(DeltaTime);
	
	if (!InventoryComponent->GetHoldingWeapon()) {
		GetCameraBoom()->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	}
	else {
		GetCameraBoom()->SetRelativeLocation(FVector(0.0f, 40.0f, 70.0f));
	}
}

void AShooterGameCharacter::CalcAnimInstanceValues(float DeltaSeconds) {

	FVector Vel = GetVelocity();
	FVector NotJumpVel = FVector(Vel.X, Vel.Y, 0.0f);

	Speed = NotJumpVel.Size();
	bDied = GetHealthComponent()->IsDead();
	bIsInAir = GetCharacterMovement()->IsFalling();

	if (!GetCharacterMovement()->IsFalling()) {

		bFinishedJumping = true;
		bJumpButtonDown = false;

	}

	AGunBase* SpawnedGun = InventoryComponent->GetCurrentGun();

	if (SpawnedGun) {
		EquippedGunFireRate = SpawnedGun->GetFireRate() + 1;
	}
	else {
		EquippedGunFireRate = 1.0f;
	}

}

float AShooterGameCharacter::CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation) const
{
	if (!Velocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = Velocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

FVector AShooterGameCharacter::GetPawnViewLocation() const
{
	if (FollowCamera)
		return FollowCamera->GetComponentLocation();
	return Super::GetPawnViewLocation();
}

// FUNCTIONS THAT REQUIRE SERVER FUNCTIONS ----------------------------------------------------------

void AShooterGameCharacter::Jump()
{
	if (!bControlsEnabled)
		return;
	
	if ((!bCanJump || !bFinishedJumping) && !bCrouching )
		return;

	if (!HasAuthority())
		ServerJump();

	if (bCrouching) {
		USCMovementComponent* SCMovement = Cast<USCMovementComponent>(GetCharacterMovement());

		if (!SCMovement)
			return;

		SCMovement->CUnCrouch();
		bCrouching = false;
	}
	else {
		Super::Jump();

		bFinishedJumping = false;
		bCanJump = false;
		bJumpButtonDown = true;

		FVector Velocity = GetCharacterMovement()->Velocity;

		if (InventoryComponent) {
			if (!InventoryComponent->GetCurrentGun()) {
				if (Velocity.Size() > 0) {
					GetCharacterMovement()->JumpZVelocity = 500;
				}
				else {
					GetCharacterMovement()->JumpZVelocity = 600;
				}
			}
			else {
				if (Velocity.Size() > 0) {
					GetCharacterMovement()->JumpZVelocity = 250;
				}
				else {
					GetCharacterMovement()->JumpZVelocity = 400;				}
			}
		}
		else {
			if (Velocity.Size() > 0) {
				GetCharacterMovement()->JumpZVelocity = 250;
			}
			else {
				GetCharacterMovement()->JumpZVelocity = 400;
			}
		}
	}
}

void AShooterGameCharacter::StopJumping()
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerStopJumping();

	Super::StopJumping();
	bJumpButtonDown = false;
	FVector Velocity = GetCharacterMovement()->Velocity;

	if (InventoryComponent) {
		if (!InventoryComponent->GetCurrentGun()) {
			if (Velocity.Size() > 0) {
				GetCharacterMovement()->JumpZVelocity = 500;
			}
			else {
				GetCharacterMovement()->JumpZVelocity = 600;
			}
		}
		else {
			if (Velocity.Size() > 0) {
				GetCharacterMovement()->JumpZVelocity = 250;
			}
			else {
				GetCharacterMovement()->JumpZVelocity = 400;
			}
		}
	}
	else {
		if (Velocity.Size() > 0) {
			GetCharacterMovement()->JumpZVelocity = 250;
		}
		else {
			GetCharacterMovement()->JumpZVelocity = 400;
		}
	}
}

void AShooterGameCharacter::Reload()
{
	if (!bControlsEnabled)
		return;

	if (!InventoryComponent->GetCurrentGun())
		return;

	if (!HasAuthority())
		ServerReload();

	if (InventoryComponent->GetCurrentGun()) {
		if (InventoryComponent->GetCurrentGun()->GetLoadedRounds() >= InventoryComponent->GetCurrentGun()->GetMagazineSize()) {
			return;
		}
	}

	if (GetWorldTimerManager().GetTimerRemaining(TimerHandle_Reloading) <= 0) {
		if (bShooting)
			StopFire();

		if (bWantsToAim)
			EndAim();

		InventoryComponent->Reload();
		bReloading = true;
		bCanFire = false;
		GetWorldTimerManager().SetTimer(TimerHandle_Reloading, this, &AShooterGameCharacter::FinishedReloading, 1.5f, false);
	}
}

void AShooterGameCharacter::FinishedReloading()
{
	if (!HasAuthority())
		ServerFinishedReloading();

	bCanFire = true;
	bReloading = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_Reloading);
}

void AShooterGameCharacter::ToggleCrouch()
{
	if (!bFinishedJumping)
		return;

	if (!bControlsEnabled)
		return;

	if (!InventoryComponent->GetCurrentGun())
		return;

	USCMovementComponent* SCMovement = Cast<USCMovementComponent>(GetCharacterMovement());

	if (!SCMovement)
		return;

	if (!HasAuthority())
		ServerToggleCrouch();

	if (bCrouching) {
		SCMovement->CUnCrouch();
		bCrouching = false;
	}
	else {
		SCMovement->CCrouch();
		bCrouching = true;
	}

}

void AShooterGameCharacter::StartFire()
{
	if (!bControlsEnabled || (bCanFire == false) || bOutOfAmmo || (!InventoryComponent->GetCurrentGun()))
		return;

	if (!HasAuthority())
		ServerStartFire();

	InventoryComponent->StartFire();
	bShooting = true;

	if (InventoryComponent->GetCurrentGun()->GetFireMode() == EFireMode::SemiAuto || InventoryComponent->GetCurrentGun()->GetFireMode() == EFireMode::SingleShot) {
		StopFire();
	}
}

void AShooterGameCharacter::StopFire()
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerStopFire();

	InventoryComponent->StopFire();
	bShooting = false;
}

void AShooterGameCharacter::BeginAim()
{
	if (!bControlsEnabled || bReloading || (!InventoryComponent->GetCurrentGun()))
		return;

	if (!HasAuthority())
		ServerBeginAim();

	if (!GetCharacterMovement()->IsFalling()) {

		bWantsToAim = true;
		EventStartAim.Broadcast(this);

	}
}

void AShooterGameCharacter::EndAim()
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerEndAim();

	bWantsToAim = false;
	EventStopAim.Broadcast(this);
}


void AShooterGameCharacter::OnCurrentGunChanged(AGunBase* NewCurrentGun)
{
	UShooterGameCharacterAnimInstance* AnimInst = Cast<UShooterGameCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInst) {
		AnimInst->OnCurrentGunChanged(NewCurrentGun);
	}
}

/// SERVER FUNCTIONS ------------------------------------------------------

void AShooterGameCharacter::ServerJump_Implementation()
{
	Jump();
}

bool AShooterGameCharacter::ServerJump_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerStopJumping_Implementation()
{
	StopJumping();
}

bool AShooterGameCharacter::ServerStopJumping_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerReload_Implementation()
{
	Reload();
}

bool AShooterGameCharacter::ServerReload_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerFinishedReloading_Implementation()
{
	FinishedReloading();
}

bool AShooterGameCharacter::ServerFinishedReloading_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerToggleCrouch_Implementation()
{
	ToggleCrouch();
}

bool AShooterGameCharacter::ServerToggleCrouch_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerStartFire_Implementation()
{
	StartFire();
}

bool AShooterGameCharacter::ServerStartFire_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerStopFire_Implementation()
{
	StopFire();
}

bool AShooterGameCharacter::ServerStopFire_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerBeginAim_Implementation()
{
	BeginAim();
}

bool AShooterGameCharacter::ServerBeginAim_Validate()
{
	return true;
}

void AShooterGameCharacter::ServerEndAim_Implementation()
{
	EndAim();
}

bool AShooterGameCharacter::ServerEndAim_Validate()
{
	return true;
}

void AShooterGameCharacter::TurnAtRate(float Rate)
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerTurnAtRate(Rate);

	if (InventoryComponent) {
		if (!InventoryComponent->GetHoldingWeapon()) {
			bUseControllerRotationYaw = false;
		}
		else {
			bUseControllerRotationYaw = true;
		}
	}
	else {
		bUseControllerRotationYaw = true;
	}

	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	FRotator TargetRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	FRotator FinalRotator = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0), TargetRotator, DeltaSecs, 15);

	AimYaw = FMath::ClampAngle(FinalRotator.Yaw, -90, 90);

}

void AShooterGameCharacter::LookUpAtRate(float Rate)
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerLookUpAtRate(Rate);

	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	FRotator TargetRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	FRotator FinalRotator = FMath::RInterpTo(FRotator(AimPitch, AimYaw, 0), TargetRotator, DeltaSecs, 15);

	AimPitch = FMath::ClampAngle(FinalRotator.Pitch, -90, 90);
}

void AShooterGameCharacter::MouseWheel(float Value)
{
	if (Value == 0)
		return;

	if (bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerMouseWheel(Value);

	if (InventoryComponent) {

		InventoryComponent->AddScrollWheelInput(Value);

	}
	
}

void AShooterGameCharacter::MoveForward(float Value)
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerMoveForward(Value);

	if (InventoryComponent) {
		if (!InventoryComponent->GetHoldingWeapon()) {
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		else {
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}


	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector NewDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(NewDirection, Value);
	}

	Direction = CalculateDirection(GetVelocity(), GetActorRotation());
}

void AShooterGameCharacter::MoveRight(float Value)
{
	if (!bControlsEnabled)
		return;

	if (!HasAuthority())
		ServerMoveRight(Value);

	if (InventoryComponent) {
		if (!InventoryComponent->GetHoldingWeapon()) {
			GetCharacterMovement()->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;
		}
		else {
			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;
		}
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector NewDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(NewDirection, Value);
	}

	Direction = CalculateDirection(GetVelocity(), GetActorRotation());
}

void AShooterGameCharacter::ServerMoveForward_Implementation(float Value)
{
	MoveForward(Value);
}

bool AShooterGameCharacter::ServerMoveForward_Validate(float Value)
{
	return true;
}

void AShooterGameCharacter::ServerMoveRight_Implementation(float Value)
{
	MoveRight(Value);
}

bool AShooterGameCharacter::ServerMoveRight_Validate(float Value)
{
	return true;
}

void AShooterGameCharacter::ServerTurnAtRate_Implementation(float Rate)
{
	TurnAtRate(Rate);
}

bool AShooterGameCharacter::ServerTurnAtRate_Validate(float Rate)
{
	return true;
}

void AShooterGameCharacter::ServerLookUpAtRate_Implementation(float Rate)
{
	LookUpAtRate(Rate);
}

bool AShooterGameCharacter::ServerLookUpAtRate_Validate(float Rate)
{
	return true;
}

void AShooterGameCharacter::ServerMouseWheel_Implementation(float Value)
{
	MouseWheel(Value);
}

bool AShooterGameCharacter::ServerMouseWheel_Validate(float Value)
{
	return true;
}

void AShooterGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterGameCharacter, bShooting);
	DOREPLIFETIME(AShooterGameCharacter, bCrouching);
	DOREPLIFETIME(AShooterGameCharacter, bJumpButtonDown);
	DOREPLIFETIME(AShooterGameCharacter, bReloading);
	DOREPLIFETIME(AShooterGameCharacter, bWantsToAim);
	DOREPLIFETIME(AShooterGameCharacter, Speed);
	DOREPLIFETIME(AShooterGameCharacter, Direction);
	DOREPLIFETIME(AShooterGameCharacter, AimYaw);
	DOREPLIFETIME(AShooterGameCharacter, AimPitch);
	DOREPLIFETIME(AShooterGameCharacter, EquippedGunFireRate);
	DOREPLIFETIME(AShooterGameCharacter, bDied);
	DOREPLIFETIME(AShooterGameCharacter, DeltaSecs);
	DOREPLIFETIME(AShooterGameCharacter, bCanJump);
	DOREPLIFETIME(AShooterGameCharacter, bFinishedJumping);
	DOREPLIFETIME(AShooterGameCharacter, bControlsEnabled);
	DOREPLIFETIME(AShooterGameCharacter, bCanFire);
	DOREPLIFETIME(AShooterGameCharacter, bOutOfAmmo);
	DOREPLIFETIME(AShooterGameCharacter, bIsInAir);

}
