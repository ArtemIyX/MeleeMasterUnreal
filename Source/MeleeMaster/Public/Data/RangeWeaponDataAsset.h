// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset.h"
#include "RangeWeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API URangeWeaponDataAsset : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	URangeWeaponDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PreAttackLen{0.5f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float AttackLen{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PostAttackLen{0.2f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range")
	FWeaponCurveData AttackCurve;
};
