// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "AnimNotifyEquipRifle.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimSequenceBase.h> 
#include "ShooterGameCharacterAnimInstance.h"

void UAnimNotifyEquipRifle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	UShooterGameCharacterAnimInstance* AnimInst = Cast<UShooterGameCharacterAnimInstance>(MeshComp->GetAnimInstance());

	if (AnimInst) {

		//AnimInst->AnimNotify_EquipRifle();

	}
}
