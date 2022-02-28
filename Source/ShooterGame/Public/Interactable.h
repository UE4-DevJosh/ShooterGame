// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class SHOOTERGAME_API AInteractable : public AActor
{
	GENERATED_BODY()

public:	
	AInteractable();
	virtual void OnPickup(class AActor* PickupInstigator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;
};
