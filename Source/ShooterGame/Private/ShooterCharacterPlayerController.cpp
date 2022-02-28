// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "ShooterCharacterPlayerController.h"
#include "ReticleWidget.h"
#include "HealthWidget.h"
#include "../ShooterGameCharacter.h"
#include "HealthComponent.h"
#include "InventoryWidget.h"
#include <Engine/Engine.h>
#include <Net/UnrealNetwork.h>
#include "ShooterGameHUD.h"
#include <GameFramework/Controller.h>
#include "GunInfo.h"
#include <GameFramework/PlayerController.h>
#include "InventoryComponent.h"

bool AShooterCharacterPlayerController::GetControlsEnabled()
{
	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetCharacter());

	if (OwningPlayer) {
		return OwningPlayer->GetControlsEnabled();
	}
	else {
		return true;
	}
}

void AShooterCharacterPlayerController::SetControlsEnabled(bool NewControlsEnabled)
{
	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetCharacter());

	if (OwningPlayer) {
		OwningPlayer->SetControlsEnabled(NewControlsEnabled);
	}
}