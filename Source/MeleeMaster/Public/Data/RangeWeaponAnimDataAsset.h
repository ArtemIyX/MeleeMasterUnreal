// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimationDataAsset.h"
#include "RangeWeaponAnimDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API URangeWeaponAnimDataAsset : public UWeaponAnimationDataAsset
{
	GENERATED_BODY()

public:
	URangeWeaponAnimDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range")
	FAttackAnimMontageData Pull;
};
