// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTERGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void EventSelectedItemChanged(int HorizontalVal, int VerticalVal);
	void EventUpdatedGunImages(TArray<class UTexture2D*> InGunImages, TArray<FString> InGunText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectedItemChanged(int HorizontalVal, int VerticalVal);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdatedGunImages(const TArray<class UTexture2D*>& InGunImages, const TArray<FString>& InGunText);
};
