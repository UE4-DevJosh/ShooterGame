// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "InventoryComponent.h"
#include "../ShooterGameCharacter.h"
#include <Engine/World.h>
#include <GameFramework/Character.h>
#include "Net/UnrealNetwork.h"
#include "ShooterCharacterPlayerController.h"
#include "HUDComponent.h"
#include "InventoryWidget.h"
#include "ShooterPlayerCharacter.h"
#include "ReticleWidget.h"
#include <GameplayStatics.generated.h>

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AShooterGameCharacter* Owner = Cast<AShooterGameCharacter>(GetOwner());

	if (GetOwnerRole() == ROLE_Authority) {

		CurrentGun = nullptr;

		if (DefaultInventory.IsValidIndex(0)) {

			for (int32 i = 0; i <= DefaultInventory.Num(); i++) {

				if (!DefaultInventory.IsValidIndex(i))
					break;

				FActorSpawnParameters SpawnParameters;

				if (Owner) {
					SpawnParameters.Owner = Owner;
				}

				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AGunBase* SpawnedGun = GetWorld()->SpawnActor<AGunBase>(DefaultInventory[i], SpawnParameters);

				AddGunToInventory(SpawnedGun);

			}
		}

		if (Owner && OwnedGuns.IsValidIndex(0)) {

			SetCurrentGun(OwnedGuns[0]);

		}
	}

	if (Owner) {
		Owner->EventStartAim.AddUniqueDynamic(this, &UInventoryComponent::StartAim);
		Owner->EventStopAim.AddUniqueDynamic(this, &UInventoryComponent::StopAim);
	}
	
	if (CurrentGun) {
		bHoldingWeapon = true;
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(Owner->GetController());
		if (OwningController) {
			if (OwningController->IsLocalController()) {
				UReticleWidget* NewGunReticle = CurrentGun->GetReticle();
				if (NewGunReticle) {
					AddedShotSizeAiming = NewGunReticle->AddedShotSizeAiming;
					AddedShotSizeNonAiming = NewGunReticle->AddedShotSizeNonAiming;
					MinimumNonAimingSize = NewGunReticle->MinimumNonAimingSize;
					MaximumNonAimingSize = NewGunReticle->MaximumNonAimingSize;
					MinimumAimingSize = NewGunReticle->MinimumAimingSize;
					MaximumAimingSize = NewGunReticle->MaximumAimingSize;
				}
			}
		}
	}

	if (Owner) {
		AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(Owner);

		if (PlOwner) {
			GunArrays.Empty(GunArrays.Num());
			for (int32 i = 0; i <= 5; i++)
			{
				FGunArray Arr = FGunArray();
				GunArrays.Add(i, Arr);
			}
			//Initialize the SelectedSlot TMap if there's nothing in it
			if (!SelectedSlot.Contains(0)) {
				for (int32 i = 0; i <= 5; i++)
				{
					SelectedSlot.Add(i, 0);
				}
			}
		}
	}
}

class AGunBase* UInventoryComponent::GetCurrentGun()
{
	if (CurrentGun) { return CurrentGun; }
	else { return nullptr; }
}

void UInventoryComponent::TriggerFireEvent()
{
	EventFire.Broadcast(this);

	float Reticle_X = FMath::Clamp((ReticleSize.X + ShotAddedSize), MinimumSize, MaximumSize);
	float Reticle_Y = FMath::Clamp((ReticleSize.Y + ShotAddedSize), MinimumSize, MaximumSize);

	ReticleSize = FVector2D(Reticle_X, Reticle_Y);
}

void UInventoryComponent::StartFire()
{
	if (CurrentGun) {

		CurrentGun->StartFire();
		EventStartFire.Broadcast(this);

	}
	bFiring = true;
}

void UInventoryComponent::StopFire()
{
	if (CurrentGun) {

		CurrentGun->StopFire();
		EventStopFire.Broadcast(this);
	}
	bFiring = false;
}

void UInventoryComponent::Reload()
{
	if (!CurrentGun)
		return;

	if (CurrentGun->GetLoadedRounds() < CurrentGun->GetMagazineSize()) {

		CurrentGun->Reload();
		EventReload.Broadcast(this);
	}
}

void UInventoryComponent::SetCurrentGun(AGunBase* NewGun)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerSetCurrentGun(NewGun);

	AShooterGameCharacter* Owner = Cast<AShooterGameCharacter>(GetOwner());

	if (OwnedGuns.Contains(NewGun)) {

		if (Owner) {

			if (CurrentGun) {
				CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				CurrentGun->SetActorLocation(FVector(0.0f, 0.0f, -2000));
			}

			bHoldingWeapon = true;
			CurrentGun = NewGun;
			FName GunBoneSocketName = CurrentGun->GetGunBoneSocketName();
			CurrentGun->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GunBoneSocketName);
			Owner->OnCurrentGunChanged(CurrentGun);

			if (CurrentGun->GetLoadedRounds() > 0) {
				Owner->SetOutOfAmmo(false);
			}
			else {
				Owner->SetOutOfAmmo(true);
			}

			AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(Owner->GetController());
			if (OwningController) {
				if (OwningController->IsLocalController()) {
					UReticleWidget* NewGunReticle = NewGun->GetReticle();
					if (NewGunReticle) {
						AddedShotSizeAiming = NewGunReticle->AddedShotSizeAiming;
						AddedShotSizeNonAiming = NewGunReticle->AddedShotSizeNonAiming;
						MinimumNonAimingSize = NewGunReticle->MinimumNonAimingSize;
						MaximumNonAimingSize = NewGunReticle->MaximumNonAimingSize;
						MinimumAimingSize = NewGunReticle->MinimumAimingSize;
						MaximumAimingSize = NewGunReticle->MaximumAimingSize;
					}
				}
			}
			MulticastEquippedGunChanged(CurrentGun);
		}
	}
}

void UInventoryComponent::AddGunToInventory(AGunBase* NewGun)
{
	if (!NewGun)
		return;

	if (GetOwnerRole() < ROLE_Authority)
		ServerAddGunToInventory(NewGun);

	AShooterGameCharacter* Owner = Cast<AShooterGameCharacter>(GetOwner());

	if (Owner) {
		NewGun->SetActorLocation(FVector(0.0f, 0.0f, -1000));

		if (!NewGun->GetOwner())
			NewGun->SetOwner(Owner);

		OwnedGuns.Add(NewGun);

		if (!CurrentGun) {
		
			SetCurrentGun(NewGun);

		}

		EventChangedOwnedGuns.Broadcast(this, OwnedGuns);

		//0 Heavies, 1 Snipers, 2 Pistols, 3 Shotguns, 4 SMGs, 5 ARs

		AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(Owner);
		if (PlOwner) {
			int WeaponClassInt = GetIntFromWeaponClass(NewGun->GetWeaponClass());

			if (GunArrays.Find(WeaponClassInt)) {
				if (GunArrays[WeaponClassInt].Guns.Num() == 0) {
					SetVerticalSelectedSlot(WeaponClassInt, 1);
				}
				GunArrays[WeaponClassInt].Guns.Add(NewGun);
			}
		}
	}
}

void UInventoryComponent::RemoveGunFromInventory(AGunBase* RemovedGun)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerRemoveGunFromInventory(RemovedGun);

	if (RemovedGun == CurrentGun) {
		OwnedGuns.Remove(RemovedGun);
		SetCurrentGun(OwnedGuns[0]);
		RemovedGun->Destroy(false, false);
	}
	else {
		OwnedGuns.Remove(RemovedGun);
		RemovedGun->Destroy(false, false);
	}

	EventChangedOwnedGuns.Broadcast(this, OwnedGuns);

	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		int WeaponClassInt = GetIntFromWeaponClass(RemovedGun->GetWeaponClass());

		GunArrays[WeaponClassInt].Guns.Remove(RemovedGun);

		if (GunArrays[WeaponClassInt].Guns.Num() == 0) {
			SetVerticalSelectedSlot(WeaponClassInt, 0);
		}
	}
}

void UInventoryComponent::ServerSetCurrentGun_Implementation(AGunBase* NewGun)
{
	SetCurrentGun(NewGun);
}

bool UInventoryComponent::ServerSetCurrentGun_Validate(AGunBase* NewGun)
{
	return true;
}

void UInventoryComponent::ServerAddGunToInventory_Implementation(AGunBase* NewGun)
{
	AddGunToInventory(NewGun);
}

bool UInventoryComponent::ServerAddGunToInventory_Validate(AGunBase* NewGun)
{
	return true;
}

void UInventoryComponent::ServerRemoveGunFromInventory_Implementation(AGunBase* RemovedGun)
{
	RemoveGunFromInventory(RemovedGun);
}

bool UInventoryComponent::ServerRemoveGunFromInventory_Validate(AGunBase* RemovedGun)
{
	return true;
}

void UInventoryComponent::MulticastEquippedGunChanged_Implementation(class AGunBase* NewCurrentGun)
{
	EventChangedEquippedGun.Broadcast(this, NewCurrentGun);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, CurrentGun);
	DOREPLIFETIME(UInventoryComponent, OwnedGuns);
	DOREPLIFETIME(UInventoryComponent, bFiring);
	DOREPLIFETIME(UInventoryComponent, bHit);
	DOREPLIFETIME(UInventoryComponent, bAiming);
	DOREPLIFETIME(UInventoryComponent, ReticleSize);
	DOREPLIFETIME(UInventoryComponent, bHoldingWeapon);
}

// RETICLE STUFF ----------------------------------------------------------------------------------------------------------------------------------------------------------------

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFiring == false && ReticleSize.X != MinimumSize) {

		if (MinimumSize > ReticleSize.X) {
			float Reticle_X = FMath::FInterpTo(ReticleSize.X, MinimumSize, DeltaTime, InterpSpeed * 3);
			float Reticle_Y = FMath::FInterpTo(ReticleSize.Y, MinimumSize, DeltaTime, InterpSpeed * 3);

			ReticleSize = FVector2D(Reticle_X, Reticle_Y);
		}
		else {
			float Reticle_X = FMath::FInterpTo(ReticleSize.X, MinimumSize, DeltaTime, InterpSpeed);
			float Reticle_Y = FMath::FInterpTo(ReticleSize.Y, MinimumSize, DeltaTime, InterpSpeed);

			ReticleSize = FVector2D(Reticle_X, Reticle_Y);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////// 
/// 
/// InvWid
/// 
/// 

int32 UInventoryComponent::GetVerticalSelectedSlot(int32 InHorizontalSlot)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		if (SelectedSlot.Contains(InHorizontalSlot))
			return SelectedSlot[InHorizontalSlot];
		else
			return 0;
	}
	else
		return 0;
}

void UInventoryComponent::SetVerticalSelectedSlot(int32 InHorizontalSlot, int32 InNewVerticalSlot)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		SelectedSlot[InHorizontalSlot] = InNewVerticalSlot;
	}
}

void UInventoryComponent::SetHorizontalSelectedSlot(int32 NewVal)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		HorizontalSelectedSlot = NewVal;
	}
}

int32 UInventoryComponent::GetCurrentGunFromGunArray(int32 InWeaponClass)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		if (GunArrays.Contains(InWeaponClass)) {

			TArray<AGunBase*> Guns = GunArrays[InWeaponClass].Guns;

			if (Guns.Num() > 0) {

				for (int32 i = 0; i < Guns.Num(); i++)
				{
					if (Guns[i] == CurrentGun)
						return i;
					else
						continue;
				}
				return -1;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else
		return -1;
}

void UInventoryComponent::UpdateGunImages()
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		TArray<UTexture2D*> NewGunImages;
		TArray<FString> NewGunText;

		if (!CurrentGun) {
			NewGunImages.Add(DefaultGunImage);
			NewGunImages.Add(DefaultGunImage);
			NewGunImages.Add(DefaultGunImage);
			NewGunImages.Add(DefaultGunImage);
			NewGunImages.Add(DefaultGunImage);
			NewGunImages.Add(DefaultGunImage);

			NewGunText.Add(FString("Heavy: 0/0"));
			NewGunText.Add(FString("Sniper: 0/0"));
			NewGunText.Add(FString("Pistol: 0/0"));
			NewGunText.Add(FString("Shotgun: 0/0"));
			NewGunText.Add(FString("Submachine: 0/0"));
			NewGunText.Add(FString("Assault: 0/0"));

			AShooterPlayerCharacter* OwningCharacter = Cast<AShooterPlayerCharacter>(GetOwner());

			if (OwningCharacter) {
				AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningCharacter->GetController());
				if (OwningController) {
					if (OwningController->IsLocalController()) {
						UHUDComponent* HUDComp = OwningCharacter->GetHUDComponent();
						if (HUDComp) {
							UInventoryWidget* InvWid = HUDComp->GetInventoryWidget();
							if (InvWid) {
								InvWid->EventUpdatedGunImages(NewGunImages, NewGunText);
							}
						}
					}
				}
			}
			return;
		}

		//Initialize the SelectedSlot TMap if there's nothing in it
		if (!SelectedSlot.Contains(0)) {
			for (int32 i = 0; i <= 5; i++)
			{
				SelectedSlot.Add(i, 0);
			}
		}

		//Get Selected Weapon of that class and return the gun image

				//NewGunImages.Add(NewGunBase->GetGunImage());	
				//NewGunImages.Add(DefaultGunImage);

		for (int32 i = 0; i <= 5; i++)
		{

			int Selected = 0;
			int MaxRange = 0;

			int VSelectedSlot = GetVerticalSelectedSlot(i) - 1;

			if (GunArrays.Contains(i)) {

				if (GunArrays[i].Guns.IsValidIndex(VSelectedSlot)) {
					AGunBase* GunWithImage = GunArrays[i].Guns[VSelectedSlot];
					Selected = VSelectedSlot + 1;
					NewGunImages.Add(GunWithImage->GetGunImage());
					MaxRange = GunArrays[i].Guns.Num();
				}
				else {
					NewGunImages.Add(DefaultGunImage);
				}
			}
			else {
				NewGunImages.Add(DefaultGunImage);
			}



			if (Selected == 0 && MaxRange > 0)
				Selected++;

			const TEnumAsByte<EWeaponClass> WC = GetWeaponClassFromInt(i);

			FString StrWC = UEnum::GetValueAsString(WC.GetValue());

			StrWC.Append(": ");

			FString StrSelected = FString::FromInt(Selected);
			FString StrMaxRange = FString::FromInt(MaxRange);

			StrWC.Append(StrSelected);
			StrWC.Append("/");
			StrWC.Append(StrMaxRange);

			NewGunText.Add(StrWC);

		}

		AShooterPlayerCharacter* OwningCharacter = Cast<AShooterPlayerCharacter>(GetOwner());

		if (OwningCharacter) {
			AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningCharacter->GetController());
			if (OwningController) {
				if (OwningController->IsLocalController()) {
					UHUDComponent* HUDComp = OwningCharacter->GetHUDComponent();
					if (HUDComp) {
						UInventoryWidget* InvWid = HUDComp->GetInventoryWidget();
						if (InvWid) {
							InvWid->EventUpdatedGunImages(NewGunImages, NewGunText);
						}
					}
				}
			}
		}
	}
}


void UInventoryComponent::AddScrollWheelInput(float AxisVal)
{
	if (AxisVal == 0)
		return;

	if (!CurrentGun)
		return;

	int32 MaxRange = GunArrays[HorizontalSelectedSlot].Guns.Num();

	if (MaxRange == 0)
		return;

	int32 NewVerticalSlot = GetVerticalSelectedSlot(HorizontalSelectedSlot);
	NewVerticalSlot++;

	if (NewVerticalSlot > MaxRange)
		NewVerticalSlot = 1;
	else if (NewVerticalSlot <= 0)
		NewVerticalSlot = MaxRange;

	SetVerticalSelectedSlot(HorizontalSelectedSlot, NewVerticalSlot);

	UpdateGunImages();

	int32 GunIndex = NewVerticalSlot;
	GunIndex--;

	AGunBase* NewCurrentGun = GunArrays[HorizontalSelectedSlot].Guns[GunIndex];

	SetCurrentGun(NewCurrentGun);

	AShooterPlayerCharacter* OwningCharacter = Cast<AShooterPlayerCharacter>(GetOwner());

	if (OwningCharacter) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningCharacter->GetController());
		if (OwningController) {
			if (OwningController->IsLocalController()) {
				UHUDComponent* HUDComp = OwningCharacter->GetHUDComponent();
				if (HUDComp) {
					UInventoryWidget* InvWid = HUDComp->GetInventoryWidget();
					if (InvWid) {
						InvWid->EventSelectedItemChanged(HorizontalSelectedSlot, NewVerticalSlot);
					}
				}
			}
		}
	}
}

EWeaponClass UInventoryComponent::GetWeaponClassFromInt(int32 WeaponClassInt)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {

		switch (WeaponClassInt)
		{
		case 0:
			return Heavy;
		case 1:
			return Sniper;
		case 2:
			return Pistol;
		case 3:
			return Shotgun;
		case 4:
			return Submachine;
		case 5:
			return Assault;
		default:
			return Heavy;
		}
	}
	else
		return Heavy;
}

int32 UInventoryComponent::GetIntFromWeaponClass(EWeaponClass InWeaponClass)
{
	AShooterPlayerCharacter* PlOwner = Cast<AShooterPlayerCharacter>(GetOwner());
	if (PlOwner) {
		switch (InWeaponClass)
		{
		case Heavy:
			return 0;
		case Sniper:
			return 1;
		case Pistol:
			return 2;
		case Shotgun:
			return 3;
		case Submachine:
			return 4;
		case Assault:
			return 5;
		default:
			return 0;
		}
	}
	else
		return -1;
}

// INV WIDGET STUFF ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// RETICLE WIDGET STUFF ----------------------------------------------------------------------------------------------------------------------------------------------------------------

void UInventoryComponent::SetHit()
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerSetHit();

	if (GetOwner()) {
		bHit = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Hit, this, &UInventoryComponent::ChangeHit, 0.2f, false);
	}
}

void UInventoryComponent::ServerSetHit_Implementation()
{
	SetHit();
}

bool UInventoryComponent::ServerSetHit_Validate()
{
	return true;
}

void UInventoryComponent::SetReticleSize(FVector2D NewSize)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerSetReticleSize(NewSize);

	ReticleSize = NewSize;
}

void UInventoryComponent::ServerSetReticleSize_Implementation(FVector2D NewSize)
{
	SetReticleSize(NewSize);
}

bool UInventoryComponent::ServerSetReticleSize_Validate(FVector2D NewSize)
{
	return true;
}

void UInventoryComponent::ChangeHit()
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerChangeHit();

	bHit = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Hit);
}

void UInventoryComponent::ServerChangeHit_Implementation()
{
	ChangeHit();
}

bool UInventoryComponent::ServerChangeHit_Validate()
{
	return true;
}

void UInventoryComponent::StartAim(AShooterGameCharacter* ShooterGameChar)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerStartAim(ShooterGameChar);

	MinimumSize = MinimumAimingSize;
	MaximumSize = MaximumAimingSize;
	ShotAddedSize = AddedShotSizeAiming;
	bAiming = true;
}

void UInventoryComponent::ServerStartAim_Implementation(class AShooterGameCharacter* ShooterGameChar)
{
	StartAim(ShooterGameChar);
}

bool UInventoryComponent::ServerStartAim_Validate(class AShooterGameCharacter* ShooterGameChar)
{
	return true;
}

void UInventoryComponent::StopAim(AShooterGameCharacter* ShooterGameChar)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerStopAim(ShooterGameChar);

	MinimumSize = MinimumNonAimingSize;
	MaximumSize = MaximumNonAimingSize;
	ShotAddedSize = AddedShotSizeNonAiming;
	bAiming = false;
}

void UInventoryComponent::ServerStopAim_Implementation(class AShooterGameCharacter* ShooterGameChar)
{
	StopAim(ShooterGameChar);
}

bool UInventoryComponent::ServerStopAim_Validate(class AShooterGameCharacter* ShooterGameChar)
{
	return true;
}

// INV WIDGET STUFF ----------------------------------------------------------------------------------------------------------------------------------------------------------------

// RETICLE WIDGET STUFF ----------------------------------------------------------------------------------------------------------------------------------------------------------------