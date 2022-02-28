// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GunImageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UGunImageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTexture2D* GetImage() { return Image; };

	UFUNCTION(BlueprintCallable)
	void SetImage(UTexture2D* NewVar) { Image = NewVar; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetGunText() { return GunText; };

	UFUNCTION(BlueprintCallable)
	void SetGunText(FString NewVar) { GunText = NewVar; };

protected:
	UPROPERTY()
	UTexture2D* Image;

	UPROPERTY()
	FString GunText;


};
