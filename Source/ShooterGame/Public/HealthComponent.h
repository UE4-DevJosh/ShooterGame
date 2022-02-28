// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//DYNAMIC DELEGATE FOR FOnHealthChangedSignature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComponent, float, Health, float, DeltaHealth, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//CLASS CONSTRUCTOR
	UHealthComponent();

	//BLUEPRINT ASSIGNABLE EVENT FOR FOnHealthChangedSignature
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent")
	FOnHealthChangedSignature OnHealthChanged;

	//FLOAT GETTER METHODS
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return Health; };

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() { return DefaultHealth; };

	//BOOL GETTER CHECKING IF OWNER IS ALIVE
	UFUNCTION(BlueprintCallable)
	bool IsDead() { return bDied; };

protected:
	//OVERIDING PARENT METHOD
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnRep_Health();

	//DEFAULT HEALTH FLOAT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	//DEALTH ANIMATION ASSET
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimationAsset* DeathAnimation;

	//DEFAULT DELEGATE FOR FOnHealthChangedSignature
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//HEALTH FLOAT
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;
		
};
