// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameGameMode.generated.h"

UCLASS(minimalapi)
class AShooterGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShooterGameGameMode();
	void EndGame();
	void LevelComplete();
	void LoadNextLevel();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> DefaultLevelCompleteWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	class UUserWidget* LevelCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> DefaultGameCompleteWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	class UUserWidget* GameCompleteWidget;

	FTimerHandle LevelSwapTimer;

private:
	virtual void BeginPlay() override;

	UPROPERTY()
	class AShooterCharacterPlayerController* PlayerCont;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Levels")
	FName LevelString;

};



