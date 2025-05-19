// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDataAsset.h"
#include "RangeWeaponDataAsset.generated.h"


class ABasicProjectile;

/**
 * @class URangeWeaponDataAsset
 * @brief Data asset for configuring long-range weapons such as bows and crossbows.
 * @see UWeaponDataAsset
 * @see URangeWeaponAnimDataAsset
 * 
 * This class inherits from UWeaponDataAsset and provides properties to customize the behavior and visuals of long-range weapons.
 * It is used to define attributes like damage, attack timing, animation curves, and visual elements for weapons like bows and crossbows.
 * For animations, this asset requires a corresponding URangeWeaponAnimDataAsset to define animation sequences and blends.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API URangeWeaponDataAsset : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor for URangeWeaponDataAsset.
	 */
	URangeWeaponDataAsset();

public:
	/**
	 * @brief Base damage dealt by the weapon per shot.
	 * @note Must be greater than or equal to 0.01.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float BasicDamage;
	
	/**
	 * @brief Duration of the pre-attack phase (e.g., drawing the bowstring) in seconds.
	 * @note Must be greater than or equal to 0.01. Default value is 0.5 seconds.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PreAttackLen{0.5f};

	/**
	 * @brief Duration of the post-attack phase (e.g., recovery after releasing the bowstring) in seconds.
	 * @note Must be greater than or equal to 0.01. Default value is 0.2 seconds.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range", meta=(ClampMin="0.01"))
	float PostAttackLen{0.2f};
	
	/**
	 * @brief Curve data for the attack, defining the progression of the attack animation.
	 * @note Requires a corresponding URangeWeaponAnimDataAsset for animation sequences.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range")
	FWeaponCurveData AttackCurve;
	
	/**
	 * @brief Visual data for the arrow displayed in first-person view when the bowstring is pulled.
	 * @note This represents the local arrow model visible in the bow during the aiming phase.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Range|Visual")
	FBowArrowData Arrow;

public:
	
};
