// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#include "ShooterGameGameMode.h"
#include "ShooterCharacterPlayerController.h"
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>
#include "ShooterGameCharacter.h"
#include <GameFramework/NavMovementComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

AShooterGameGameMode::AShooterGameGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShooterGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerCont = Cast<AShooterCharacterPlayerController>(GetWorld()->GetFirstPlayerController());

	FInputModeGameAndUI InputMode;
	PlayerCont->SetInputMode(InputMode);
	PlayerCont->bShowMouseCursor = false;

	if (DefaultLevelCompleteWidget)
		LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultLevelCompleteWidget);
	if (DefaultGameCompleteWidget)
		GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultGameCompleteWidget);

	LoadNextLevel();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play"));

}

void AShooterGameGameMode::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("End Game"));

	if (LevelCompleteWidget) {
		LevelCompleteWidget->RemoveFromParent();

		if (GameCompleteWidget) {
			GameCompleteWidget->AddToViewport();

			PlayerCont->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			PlayerCont->SetInputMode(InputMode);
		}
	}

	AShooterGameCharacter* OwningPlayer = Cast<AShooterGameCharacter>(PlayerCont->GetPawn());
	if (OwningPlayer) {

		UCharacterMovementComponent* OwnerCharMoveComp = OwningPlayer->GetCharacterMovement();
		UCapsuleComponent* OwnerCapsuleComp = Cast<UCapsuleComponent>(OwningPlayer->GetCapsuleComponent());

		OwningPlayer->SetCanFire(false);

		if (OwnerCharMoveComp) {
			OwnerCharMoveComp->StopMovementImmediately();
		}

		if (OwnerCapsuleComp) {
			OwnerCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		OwningPlayer->DetachFromControllerPendingDestroy();
		OwningPlayer->SetLifeSpan(10.0f);

	}

}

void AShooterGameGameMode::LevelComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("Level Complete"));

	if (LevelCompleteWidget)
		LevelCompleteWidget->AddToViewport();

	GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &AShooterGameGameMode::LoadNextLevel, 2.0f, false);

}

void AShooterGameGameMode::LoadNextLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Load Next Level"));

	if (LevelString != "") {
		//UGameplayStatics::OpenLevel(this, LevelString, true);
	}
	else {
		EndGame();
	}
}
