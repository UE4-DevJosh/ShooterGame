// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "HealthWidget.h"
#include "ShooterPlayerCharacter.h"
#include "HealthComponent.h"
#include <GameFramework/Actor.h>
#include <Net/UnrealNetwork.h>


void UHealthWidget::EventTakeDamage_Implementation(class UHealthComponent* OwningHealthComponent, float NewHealth, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser)
{
	SetHealth(NewHealth);
	SetMaxHealth(OwningHealthComponent->GetMaxHealth());
}

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AShooterPlayerCharacter* OwnerPlayer = Cast<AShooterPlayerCharacter>(GetOwningPlayerPawn());

	if (OwnerPlayer) {

		UHealthComponent* HealthComponent = Cast<UHealthComponent>(OwnerPlayer->GetComponentByClass(UHealthComponent::StaticClass()));

		if (HealthComponent) {

			Health = HealthComponent->GetHealth();
			MaxHealth = HealthComponent->GetMaxHealth();

			HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &UHealthWidget::EventTakeDamage);

		}
	}
}