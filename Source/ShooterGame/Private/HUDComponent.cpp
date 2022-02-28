// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "HUDComponent.h"
#include "ReticleWidget.h"
#include "HealthWidget.h"
#include "InventoryWidget.h"
#include "GunInfo.h"
#include "../ShooterGameCharacter.h"
#include "ShooterCharacterPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include "ReticleManagerWidget.h"
#include "InventoryComponent.h"

UHUDComponent::UHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());
	if (OwningPlayer) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());
		if (OwningController) {
			ReticleManagerWidget = CreateWidget<UReticleManagerWidget>(OwningController, ReticleManagerWidgetSubclass);
			HealthWidget = CreateWidget<UHealthWidget>(OwningController, HealthWidgetSubclass);
			InventoryWidget = CreateWidget<UInventoryWidget>(OwningController, InventoryWidgetSubclass);
			GunInfoWidget = CreateWidget<UGunInfo>(OwningController, GunInfoWidgetSubclass);

			InventoryComponent = OwningPlayer->GetInventoryComponent();

			if (InventoryComponent) {
				Setup();
			}
		}
	}
}

void UHUDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	if (OwningPlayer) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());
		if (OwningController)
			if (OwningController->IsLocalController())
				if (ReticleManagerWidget)
					ReticleManagerWidget->TickEvent(DeltaTime);
	}
}

void UHUDComponent::Setup() {
	if (ReticleManagerWidget)
		ReticleManagerWidget->AddToViewport(0);
	if (HealthWidget)
		HealthWidget->AddToViewport(0);
	if (GunInfoWidget)
		GunInfoWidget->AddToViewport(0);

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());
	if (OwningPlayer) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());
		if (OwningController)
			OwningController->SetInputMode(FInputModeGameOnly());
	}

	if (InventoryComponent) {
		if (GunInfoWidget) {
			GunInfoWidget->SetInventoryComponent(InventoryComponent);
			GunInfoWidget->Setup();
		}
	}
}

void UHUDComponent::ShowInventory()
{
	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	if (OwningPlayer) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());
		if (OwningController) {
			if (OwningController->IsLocalController()) {
				OwningPlayer->EndAim();
				OwningPlayer->StopFire();

				if (InventoryWidget && InventoryComponent) {
					InventoryWidget->AddToViewport(0);
					InventoryComponent->InvWidInViewport = true;

					OwningController->SetInputMode(FInputModeGameAndUI());
					OwningController->bShowMouseCursor = true;

					OwningPlayer->SetControlsEnabled(false);

					ReticleManagerWidget->RemoveFromViewport();
					GunInfoWidget->RemoveFromViewport();
					HealthWidget->RemoveFromViewport();

					InventoryComponent->UpdateGunImages();
				}
			}
		}
	}
}

void UHUDComponent::HideInventory()
{
	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(GetOwner());

	if (OwningPlayer) {
		AShooterCharacterPlayerController* OwningController = Cast<AShooterCharacterPlayerController>(OwningPlayer->GetController());
		if (OwningController) {
			if (OwningController->IsLocalController()) {
				if (InventoryWidget && InventoryComponent) {
					InventoryWidget->RemoveFromViewport();
					InventoryComponent->InvWidInViewport = false;
					OwningPlayer->SetControlsEnabled(true);

					OwningController->SetInputMode(FInputModeGameOnly());
					OwningController->bShowMouseCursor = false;

					ReticleManagerWidget->AddToViewport(0);
					GunInfoWidget->AddToViewport(0);
					HealthWidget->AddToViewport(0);
				}
			}
		}
	}
}