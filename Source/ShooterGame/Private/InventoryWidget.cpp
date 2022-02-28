// Created by Joshua Hines. All Rights Reserved. ShooterGame was created with Unreal Engine 4.


#include "InventoryWidget.h"
#include "GunImageWidget.h"
#include "GunBase.h"
#include <Engine/Texture2D.h>
#include <Containers/Array.h>
#include <Containers/UnrealString.h>
#include <UObject/Class.h>
#include "ShooterPlayerCharacter.h"
#include "InventoryComponent.h"
#include "GunBase.h"

void UInventoryWidget::EventSelectedItemChanged(int HorizontalVal, int VerticalVal)
{
	OnSelectedItemChanged(HorizontalVal, VerticalVal);
}

void UInventoryWidget::EventUpdatedGunImages(TArray<UTexture2D*> InGunImages, TArray<FString> InGunText)
{
	OnUpdatedGunImages(InGunImages, InGunText);
}
