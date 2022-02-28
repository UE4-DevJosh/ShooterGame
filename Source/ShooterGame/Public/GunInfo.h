// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunInfo.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UGunInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void SetInventoryComponent(class UInventoryComponent* NewInvComp) { InvComp = NewInvComp; };

	UFUNCTION()
	void EventLoadedRoundsChanged(class UInventoryComponent* InInvComp, int LoadedRounds, int MagazineSize);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(int LoadedRounds, int MagazineSize);

	UPROPERTY()
	class UInventoryComponent* InvComp;
};
