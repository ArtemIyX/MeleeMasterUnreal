// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponAnimationDataAsset.generated.h"


/**
 * @class UWeaponAnimationDataAsset
 * @brief Asset class that stores animation data for weapons, including equip, de-equip, idle, and aim animations.
 * This class is used to define animations for different weapon actions like equipping, idle, and aiming.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponAnimationDataAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UWeaponAnimationDataAsset();

public:
	/**
	 * @brief Animation montage data for equipping the weapon.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Manage")
	FAnimMontageFullData Equip;

	/**
	 * @brief Animation montage data for de-equipping the weapon.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Manage")
	FAnimMontageFullData DeEquip;

	/**
	 * @brief Sequence data for the weapon's idle animation.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Misc")
	FSequenceData Idle;

	/**
	 * @brief Aim offset blend space used for weapon aiming.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Misc")
	UAimOffsetBlendSpace1D* AimOffset;
};
