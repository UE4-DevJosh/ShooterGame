// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHUDComponent(); 

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Setup();
	void ShowInventory();
	void HideInventory();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UHealthWidget* GetHealthWidget() { return HealthWidget; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UReticleManagerWidget* GetReticleManagerWidget() { return ReticleManagerWidget; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UGunInfo* GetGunInfoWidget() { return GunInfoWidget; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryWidget* GetInventoryWidget() { return InventoryWidget; };

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UReticleManagerWidget> ReticleManagerWidgetSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UInventoryWidget> InventoryWidgetSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UHealthWidget> HealthWidgetSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGunInfo> GunInfoWidgetSubclass;

private:
	//Widgets
	UPROPERTY()
	class UHealthWidget* HealthWidget;
	UPROPERTY()
	class UReticleManagerWidget* ReticleManagerWidget;
	UPROPERTY()
	class UGunInfo* GunInfoWidget;
	UPROPERTY()
	class UInventoryWidget* InventoryWidget;
	UPROPERTY()
	class UInventoryComponent* InventoryComponent;
};
