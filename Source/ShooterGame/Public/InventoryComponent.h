// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunBase.h"
#include "InventoryComponent.generated.h"

USTRUCT()
struct FGunArray
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<AGunBase*> Guns;
};

struct FSortByWeaponClass
{
	FSortByWeaponClass(EWeaponClass InWeaponClassInt)
		: WeaponClassIndex(InWeaponClassInt)
	{

	}

	EWeaponClass WeaponClassIndex;

	bool operator()(AGunBase* A) const
	{

		EWeaponClass WeaponClassA = A->GetWeaponClass();

		return WeaponClassA == WeaponClassIndex;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventStartFire, UInventoryComponent*, InvComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventFire, UInventoryComponent*, InvComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventStopFire, UInventoryComponent*, InvComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReload, UInventoryComponent*, InvComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventChangedEquippedGun, UInventoryComponent*, InvComp, AGunBase*, NewCurrentGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventChangedOwnedGuns, UInventoryComponent*, InvComp, TArray<AGunBase*>, NewOwnedGuns);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEventLoadedRoundsChanged, UInventoryComponent*, InvComp, int, NewLoadedRounds, int, NewMagazineSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedItemChanged, int, InHorizontalVal, int, InVerticalVal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedGunImages, TArray<class UTexture2D*>, InGunImages, TArray<FString>, InGunText);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AGunBase*> GetOwnedGuns() { return OwnedGuns; };
	 
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGunBase* GetCurrentGun();

	void TriggerFireEvent();
	void StartFire();
	void StopFire();
	void Reload();
	void SetCurrentGun(AGunBase* NewGun);
	void AddGunToInventory(AGunBase* NewGun);
	void RemoveGunFromInventory(AGunBase* RemovedGun);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCurrentGun(AGunBase* NewGun);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddGunToInventory(AGunBase* NewGun);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemoveGunFromInventory(AGunBase* RemovedGun);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquippedGunChanged(AGunBase* NewCurrentGun);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventStartFire EventStartFire;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventStopFire EventStopFire;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventReload EventReload;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventFire EventFire;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventChangedEquippedGun EventChangedEquippedGun;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventChangedOwnedGuns EventChangedOwnedGuns;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEventLoadedRoundsChanged EventLoadedRoundsChanged;

protected:
	UPROPERTY(Replicated)
	AGunBase* CurrentGun;

	UPROPERTY(Replicated)
	TArray<AGunBase*> OwnedGuns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AGunBase>> DefaultInventory;


	//INVENTORY WIDGET STUFF -------------------------------------------------------------------------------------------------------------------------------------

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION(BlueprintCallable)
	void SetHorizontalSelectedSlot(int32 NewVal);

	void UpdateGunImages();
	void AddScrollWheelInput(float AxisVal);
	void SetVerticalSelectedSlot(int32 InHorizontalSlot, int32 InNewVerticalSlot);

	EWeaponClass GetWeaponClassFromInt(int32 WeaponClassInt);
	int32 GetCurrentGunFromGunArray(int32 InWeaponClass);
	int32 GetIntFromWeaponClass(EWeaponClass InWeaponClass);
	int32 GetHorizontalSelectedSlot() { return HorizontalSelectedSlot; };
	int32 GetVerticalSelectedSlot(int32 InHorizontalSlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetHoldingWeapon() { return bHoldingWeapon; };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* DefaultGunImage;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSelectedItemChanged OnSelectedItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdatedGunImages OnUpdatedGunImages;

protected:
	//0 Heavies, 1 Snipers, 2 Pistols, 3 Shotguns, 4 SMGs, 5 ARs
	UPROPERTY()
	TMap<int32, FGunArray> GunArrays;

	//0 Heavies, 1 Snipers, 2 Pistols, 3 Shotguns, 4 SMGs, 5 ARs
	UPROPERTY()
	TMap<int32, int32> SelectedSlot;

	UPROPERTY()
	int32 HorizontalSelectedSlot;

	//INV WIDGET STUFF 

	//RETICLE WIDGET STUFF ------------------------------------------------------------------------------------------------------------------------------------------
public:
	UPROPERTY()
	bool InvWidInViewport = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetFirstShotAccurate() { return FMath::IsNearlyEqual(GetReticleSize().X, MinimumAimingSize, 3.0f); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetHit() { return bHit; };

	UFUNCTION(BlueprintCallable)
	void SetHit();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetHit();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetReticleSize() { return ReticleSize; };

	UFUNCTION(BlueprintCallable)
	void SetReticleSize(FVector2D NewSize);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetReticleSize(FVector2D NewSize);

protected:
	UPROPERTY()
	FTimerHandle TimerHandle_Hit;

	void ChangeHit();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerChangeHit();

	UFUNCTION()
	void StartAim(class AShooterGameCharacter* ShooterGameChar);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAim(class AShooterGameCharacter* ShooterGameChar);

	UFUNCTION()
	void StopAim(class AShooterGameCharacter* ShooterGameChar);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopAim(class AShooterGameCharacter* ShooterGameChar);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InterpSpeed = 4.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MinimumSize = 35.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaximumSize = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinimumNonAimingSize = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaximumNonAimingSize = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinimumAimingSize = 26.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaximumAimingSize = 75.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ShotAddedSize = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AddedShotSizeAiming = 7.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AddedShotSizeNonAiming = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor IsAccurateColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor NormalColor;

private:
	UPROPERTY(Replicated)
	bool bFiring = false;

	UPROPERTY(Replicated)
	bool bHit = false;

	UPROPERTY(Replicated)
	bool bAiming = false;

	UPROPERTY(Replicated)
	bool bHoldingWeapon = false;

	UPROPERTY(Replicated)
	FVector2D ReticleSize = FVector2D(MinimumSize, MinimumSize);
};
