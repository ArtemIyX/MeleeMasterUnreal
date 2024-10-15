// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimationDataAsset.h"
#include "MeleeWeaponAnimDataAsset.generated.h"


enum class EWeaponDirection : uint8;

/**
 * @struct FMeleeAnimMontageFullData
 * @brief Struct for handling full animation montage data for melee attacks in all directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAnimMontageFullData
{
	GENERATED_BODY()

public:

	/**
	 * @brief Animation montage data for forward attack/block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Forward;

	/**
	 * @brief Animation montage data for backward attack/block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Backward;

	/**
	 * @brief Animation montage data for right attack/block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Right;

	/**
	 * @brief Animation montage data for left attack/block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Left;

	/**
	 * @brief Returns the montage data corresponding to the given direction.
	 * @param InDir The direction for which the montage data is requested.
	 * @return The animation montage data for the specified direction.
	 */
	const FAnimMontageFullData& Get(EWeaponDirection InDir);
};

/**
 * @class UMeleeWeaponAnimDataAsset
 * @brief Data asset class for handling melee weapon animation data for attack and block animations.
 * Inherits from UWeaponAnimationDataAsset.
 */
UCLASS()
class MELEEMASTER_API UMeleeWeaponAnimDataAsset : public UWeaponAnimationDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UMeleeWeaponAnimDataAsset();

public:
	/**
	 * @brief Animation montage data for melee attacks in all directions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee", meta=(ShowOnlyInnerProperties))
	FMeleeAnimMontageFullData Attack;

	/**
	 * @brief Animation montage data for melee blocks in all directions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee", meta=(ShowOnlyInnerProperties))
	FMeleeAnimMontageFullData Block;
};