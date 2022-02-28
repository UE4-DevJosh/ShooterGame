// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleManagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UReticleManagerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void TickEvent(float InDeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetHit() { return bHit; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetReticleSize() { return ReticleSize; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UReticleWidget* GetCurrentReticle() { return CurrentReticle; };

	UFUNCTION(BlueprintCallable)
	void SetCurrentReticle(class UReticleWidget* NewCurrentReticle) { CurrentReticle = NewCurrentReticle; };

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnChangeGun(class UInventoryComponent* InvComp, class AGunBase* NewCurrentGun);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReticleChange(class UReticleWidget* InReticleWidget);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bFirstShotAccurate = false;

private:
	UPROPERTY()
	bool bHit = false;

	UPROPERTY()
	FVector2D ReticleSize;

	UPROPERTY()
	class UReticleWidget* CurrentReticle;
};
