// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

//NETWORK HIT SCAN TRACE STRUCT
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AShooterCharacterPlayerController* OwningController;

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize EndTrace;


};

//WEAPON CLASS ENUM
UENUM(BlueprintType)
enum EWeaponClass
{
	Shotgun UMETA(DisplayName = "Shotgun"),
	Heavy UMETA(DisplayName = "Heavy"),
	Pistol UMETA(DisplayName = "Pistol"),
	Submachine UMETA(DisplayName = "Submachine"),
	Assault UMETA(DisplayName = "Assault"),
	Sniper UMETA(DisplayName = "Sniper"),
};

//MESH TYPE ENUM
UENUM(BlueprintType)
enum EMeshType
{
	Skeletal UMETA(DisplayName = "Skeletal"),
	Static UMETA(DisplayName = "Static"),
};

//BULLET TYPE ENUM
UENUM(BlueprintType)
enum EBulletType
{
	Projectile UMETA(DisplayName = "Projectile"),
	HitScan UMETA(DisplayName = "HitScan"),
};

//FIRE MODE ENUM
UENUM(BlueprintType)
enum EFireMode
{
	SingleShot UMETA(DisplayName = "SingleShot"),
	SemiAuto UMETA(DisplayName = "SemiAuto"),
	FullAuto UMETA(DisplayName = "FullAuto"),
};

UCLASS()
class SHOOTERGAME_API AGunBase : public AActor
{
	GENERATED_BODY()

/// 
/// GUN ATTRIBUTES
/// 

public:
	//CLASS CONSTRUCTOR
	AGunBase();
	
	//MAIN METHODS
	void StartFire();
	void StopFire();
	void Reload();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UAnimMontage* GetMeshFireMontageAiming() { return MeshFireMontageAiming; };
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UAnimMontage* GetMeshFireMontageNonAiming() { return MeshFireMontageNonAiming; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UAnimMontage* GetMeshReloadMontage() { return MeshReloadMontage; };

	//RETICLE GET METHOD
	UFUNCTION()
	UReticleWidget* GetReticle() { return GunReticle; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTexture2D* GetGunImage() { return GunImage; };

	//FNAME SOCKET GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetMuzzleSocketName() { return MuzzleSocketName; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetMagazineSocketName() { return MagazineSocketName; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetGunBoneSocketName() { return GunBoneSocketName; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetTracerTargetName() { return TracerTargetName; };

	//ENUM GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EBulletType> GetBulletType() { return BulletType; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EMeshType> GetMeshType() { return MeshType; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EFireMode> GetFireMode() { return FireMode; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EWeaponClass> GetWeaponClass() { return WeaponClass; };

	//FLOAT GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetFireRate() { return FireRate; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBaseDamage() { return BaseDamage; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetReloadTime() { return ReloadTime; };

	//INT GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetRange() { return Range; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMagazineSize() { return MagazineSize; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetLoadedRounds() { return LoadedRounds; };
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetNumRaycast() { return NumRaycast; };

protected:
	virtual void BeginPlay() override;
	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundCue* GunShotSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundAttenuation* GunShotSoundAttenuation;
	
	//PARTICLE SYSTEMS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* TracerEffect;

	//DAMAGE TYPE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UDamageType> DamageType;

	//UI IMAGE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* GunImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MeshFireMontageAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MeshFireMontageNonAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MeshReloadMontage;

	//RETICLE
	UPROPERTY()
	class UReticleWidget* GunReticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UReticleWidget> ReticleWidgetSubclass;

	UPROPERTY()
	class AShooterGameCharacter* OwnerChar;

	UPROPERTY()
	class UInventoryComponent* OwningInvComp;

	//FNAME SOCKET VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	FName MuzzleSocketName = FName("Muzzle");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	FName MagazineSocketName = FName("Magazine");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	FName GunBoneSocketName = FName("GunSocket");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	FName TracerTargetName = FName("Target");

	//ENUM VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	TEnumAsByte<EBulletType> BulletType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	TEnumAsByte<EMeshType> MeshType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	TEnumAsByte<EFireMode> FireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	TEnumAsByte<EWeaponClass> WeaponClass;

	//FLOAT VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	float FireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	float BaseDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	float ReloadTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	float BulletBloomDivisor = 30;

	//INT VARIABLES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	int Range;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	int MagazineSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = GunDefaults)
	int NumRaycast = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GunDefaults)
	int LoadedRounds;

/// 
/// GUN MECHANICS
/// 

public:
	UFUNCTION(BlueprintCallable)
	float GetLastShot() { return LastShot; };

	virtual class UStaticMeshComponent* GetBodyStatic() { return nullptr; };
	virtual class UStaticMeshComponent* GetMagazineStatic() { return nullptr; };
	virtual class USkeletalMeshComponent* GetBodySkeletal() { return nullptr; };
	virtual class USkeletalMeshComponent* GetMagazineSkeletal() { return nullptr; };

protected:
	void EndReload();
	void PlayImpactEffects(EPhysicalSurface SurfaceType, class AShooterCharacterPlayerController* OwningController, class UInventoryComponent* InInvComp);
	void PlayEffects(FVector TracerEndPoint);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMulticastEffects(FVector TracerEndPoint);

	UFUNCTION(NetMulticast, Reliable)
	void PlayMulticastSound();

	//ON REPLICATION EVENT FOR THE HIT SCAN TRACE
	UFUNCTION()
	void OnRep_HitScanTrace();

	UPROPERTY()
	FTimerHandle TimerHandle_Fire;
	UPROPERTY()
	FTimerHandle TimerHandle_Reload;
	UPROPERTY()
	FTimerHandle TimerHandle_Animation;

	//HIT SCAN TRACE
	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

private:
	UPROPERTY()
	float LastShot = 0;
	UPROPERTY()
	float ReloadedAt = 0;
	UPROPERTY()
	float LastFireTime = 0;

};
