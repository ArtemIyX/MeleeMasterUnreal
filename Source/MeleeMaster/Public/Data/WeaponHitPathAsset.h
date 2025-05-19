// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponHitPathAsset.generated.h"

/**
 * @class UWeaponHitPathAsset
 * @brief Asset used to define hit path data for melee weapons.
 * 
 * This asset memorizes the relative path of the blade with respect 
 * to the character's hand position. It is intended to be used in 
 * melee combat systems to drive accurate hit detection via traces.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponHitPathAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UWeaponHitPathAsset();

public:
	/**
	 * @brief Weapon hit data describing the shape and path of the weapon trace.
	 * 
	 * Used to define points relative to the character's hand for hit detection.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ShowOnlyInnerProperties))
	FWeaponHitData Data;

	/**
	 * @brief Radius of the trace hit box.
	 * 
	 * Defines the size of the trace used to detect hits along the path.
	 * Must be greater than 0.0001.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ClampMin="0.0001"))
	float Radius;

	/**
	 * @brief Vertical offset from the hand to adjust blade path positioning.
	 * 
	 * Typically used to fine-tune the height of the blade path relative to the character’s hand.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main")
	float ZOffset{-35.0f};

	/**
	 * @brief Type of trace used for hit detection.
	 * 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ClampMin="0.0001"))
	TEnumAsByte<ETraceTypeQuery> TraceQuery;
};
