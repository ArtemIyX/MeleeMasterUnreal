// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset.h"
#include "RangeWeaponDataAsset.generated.h"


class ABasicProjectile;
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
	float BasicDamage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PreAttackLen{0.5f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PostAttackLen{0.2f};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range")
	FWeaponCurveData AttackCurve;

	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", NoClear)
	TSubclassOf<ABasicProjectile> ProjectileClass;*/
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range|Visual")
	FBowArrowData Arrow;

public:
	
};
