// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "HealthComponent.h"
#include <GameFramework/Actor.h>
#include "HealthWidget.h"
#include "InventoryComponent.h"
#include "GunBase.h"
#include <Net/UnrealNetwork.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "../ShooterGameCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/DamageType.h>
#include "../ShooterGameGameMode.h"
#include "ShooterPlayerCharacter.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DefaultHealth = 100;

	SetIsReplicated(true);
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority) {
		AActor* MyOwner = GetOwner();

		if (MyOwner) {

			MyOwner->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::HandleTakeAnyDamage);

		}
	}

	Health = DefaultHealth;

	OnHealthChanged.Broadcast(this, Health, 0.0f, nullptr, nullptr, nullptr);
	
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!GetOwner())
		return;

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());
	if (OwningPlayer) {
		OwningPlayer->SetCanFire(false);
	}

}

void UHealthComponent::OnRep_Health()
{
	OnHealthChanged.Broadcast(this, Health, 0.0f, nullptr, nullptr, nullptr);
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0) return;

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	if (Health <= 0.0f && !IsDead()) {

		if (OwningPlayer) {
			bDied = true;
		}


		if (Cast<AShooterPlayerCharacter>(OwningPlayer)) {
			AShooterGameGameMode* GM = Cast<AShooterGameGameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
				GM->EndGame();
		}
		else {
			UCharacterMovementComponent* OwnerCharMoveComp = OwningPlayer->GetCharacterMovement();
			UCapsuleComponent* OwnerCapsuleComp = Cast<UCapsuleComponent>(OwningPlayer->GetCapsuleComponent());

			OwningPlayer->SetCanFire(false);

			if (OwnerCharMoveComp) {
				OwnerCharMoveComp->StopMovementImmediately();
			}

			if (OwnerCapsuleComp) {
				OwnerCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			OwningPlayer->DetachFromControllerPendingDestroy();
			OwningPlayer->SetLifeSpan(10.0f);
		}
	}

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, bDied);

}
