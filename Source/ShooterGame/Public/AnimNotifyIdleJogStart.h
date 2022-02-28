// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyIdleJogStart.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UAnimNotifyIdleJogStart : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
	
};
