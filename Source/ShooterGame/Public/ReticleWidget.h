// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UReticleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHit(bool bNewHit);
	void SetReticleSize(FVector2D NewReticleSize);
	void SetFirstShotAccurate(bool bNewFirstShotAccurate);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetHit() { return bHit; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetFirstShotAccurate() { return bFirstShotAccurate; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetReticleSize() { return ReticleSize; };

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeHit(bool bInHit);

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeReticleSize(FVector2D InReticleSize);

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeFirstShotAccurate(bool bInFirstShotAccurate);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinimumSize = MinimumNonAimingSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaximumSize = MaximumNonAimingSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AddedShotSizeAiming = 7.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AddedShotSizeNonAiming = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinimumNonAimingSize = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaximumNonAimingSize = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinimumAimingSize = 26.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaximumAimingSize = 75.0f;
	
private:
	bool bHit = false;
	bool bFirstShotAccurate = false;
	FVector2D ReticleSize = FVector2D(MinimumSize, MinimumSize);

};
