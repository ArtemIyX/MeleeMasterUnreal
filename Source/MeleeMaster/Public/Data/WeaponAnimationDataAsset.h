// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponAnimationDataAsset.generated.h"

class UAimOffsetBlendSpace1D;

/**
 * @struct FAnimMontageSingleData
 * @brief Struct that stores a single animation montage data for either first or third person view.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAnimMontageSingleData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor initializing the montage to nullptr and length to 1.0.
	 */
	FAnimMontageSingleData() : Value(nullptr), Length(1.0f)
	{
	}

public:
	/**
	 * @brief Soft reference to the animation montage asset.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UAnimMontage> Value;

	/**
	 * @brief Length of the montage. Minimum value is clamped to 0.01 seconds.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	float Length;
};

/**
 * @struct FAnimMontageFullData
 * @brief Struct that stores animation montage data for both first-person and third-person perspectives.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAnimMontageFullData
{
	GENERATED_BODY()

public:

	/**
	 * @brief Animation montage data for first-person view.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageSingleData FirstPerson;

	/**
	 * @brief Animation montage data for third-person view.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageSingleData ThirdPerson;
};

/**
 * @struct FSequenceData
 * @brief Struct that stores animation sequences for both first-person and third-person perspectives.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FSequenceData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Animation sequence for first-person view.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSequence* FirstPerson;
	
	/**
	 * @brief Animation sequence for third-person view.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSequence* ThirdPerson;
};


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
