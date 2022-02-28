// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "Interactable.h"
#include <GameFramework/Actor.h>
#include <Components/BoxComponent.h>
#include "Components/StaticMeshComponent.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	BoxComp->SetGenerateOverlapEvents(true);

	BoxComp->AttachTo(MeshComp);

	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AInteractable::OnPickup(AActor* PickupInstigator)
{
	if (!IsPendingKill())
		Destroy();
}
