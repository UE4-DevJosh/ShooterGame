// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "GunInteractable.h"
#include "GunBase.h"
#include "../Public/InventoryComponent.h"
#include <GameFramework/Actor.h>
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "../Public/ShooterPlayerCharacter.h"
#include "Engine/World.h"
#include <Components/BoxComponent.h>

AGunInteractable::AGunInteractable()
{
	SetReplicates(true);
	SetReplicateMovement(true);
}

void AGunInteractable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGunInteractable::OnBoxBeginOverlap);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &AGunInteractable::OnBoxEndOverlap);
	}
}

void AGunInteractable::OnPickup(AActor* PickupInstigator)
{
	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(PickupInstigator);

	if (!OwningPlayer)
		return;

	UInventoryComponent* InventoryComp = OwningPlayer->GetInventoryComponent();

	if (!InventoryComp)
		return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = OwningPlayer;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGunBase* SpawnedGun = GetWorld()->SpawnActor<AGunBase>(PickupGun, SpawnParameters);

	InventoryComp->AddGunToInventory(SpawnedGun);

	Destroy(false, false);

}

void AGunInteractable::OnPickupMulticast_Implementation(AActor* PickupInstigator)
{
	OnPickup(PickupInstigator);
}

void AGunInteractable::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AShooterGameCharacter* GameCharacter = Cast<AShooterGameCharacter>(OtherActor);
	AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(OtherActor);
	
	if (!GameCharacter)
		return;

	if (PlayerCharacter) {
		PlayerCharacter->SetAbleToPickup(true, this);
	}
	else {
		OnPickupMulticast(OtherActor);
	}
}

void AGunInteractable::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(OtherActor);

	if (!PlayerCharacter)
		return;

	PlayerCharacter->SetAbleToPickup(false, this);
}
