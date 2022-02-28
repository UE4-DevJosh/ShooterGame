// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "AnimNotifyJump.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimSequenceBase.h>
#include "ShooterGameCharacterAnimInstance.h"

void UAnimNotifyJump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UShooterGameCharacterAnimInstance* AnimInst = Cast<UShooterGameCharacterAnimInstance>(MeshComp->GetAnimInstance());

	if (AnimInst) {

		//AnimInst->AnimNotify_Jump();

	}
}
