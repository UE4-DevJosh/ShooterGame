// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return MaxHealth; };

	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; };

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; };

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth) { Health = NewHealth; };

	UFUNCTION(NetMulticast, Reliable)
	virtual void EventTakeDamage(class UHealthComponent* OwningHealthComponent, float NewHealth, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);

protected:
	UPROPERTY()
	float Health;
	UPROPERTY()
	float MaxHealth;

	virtual void NativeConstruct() override;

};
