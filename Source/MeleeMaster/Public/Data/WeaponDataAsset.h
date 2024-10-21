// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponDataAsset.generated.h"

class UWeaponAnimationDataAsset;
class AWeaponVisual;
class UAbstractWeapon;
/**
 * @class UWeaponDataAsset
 * @brief Asset class that contains data for weapon configuration.
 * It defines the class of the weapon, its visuals, animations, and timing for equip and de-equip actions.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponDataAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UWeaponDataAsset();

public:
	/**
	 * @brief The class of the weapon that this data asset represents.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main")
	TSubclassOf<UAbstractWeapon> WeaponClass;

	/**
	 * @brief A list of visual representations (meshes) of the weapon.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visual")
	TArray<TSubclassOf<AWeaponVisual>> Visuals;

	/**
	 * @brief Animation data asset for the weapon.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visual")
	UWeaponAnimationDataAsset* Animations;

	/**
	 * @brief The time taken to equip the weapon (in seconds).
	 * @details The minimum value is clamped at 0.01 seconds.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Timing", meta=(ClampMin="0.01"))
	float EquipTime;

	/**
	 * @brief The time taken to de-equip the weapon (in seconds).
	 * @details The minimum value is clamped at 0.01 seconds.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Timing", meta=(ClampMin="0.01"))
	float DeEquipTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Damage", meta=(ClampMin="0.01"))
	EWeaponTier WeaponTier;
public:
	/**
	 * @brief Checks if the data asset is valid for weapon creation.
	 * @return True if the asset is valid for creating a weapon, false otherwise.
	 */
	virtual bool IsValidToCreate() const;
};
