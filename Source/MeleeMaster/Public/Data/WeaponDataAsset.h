﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponDataAsset.generated.h"

class AWeaponVisual;
class UAbstractWeapon;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponDataAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	UWeaponDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main")
	TSubclassOf<UAbstractWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visual")
	TMap<FName, TSubclassOf<AWeaponVisual>> Visuals;

public:
	bool IsValidToCreate() const;
};