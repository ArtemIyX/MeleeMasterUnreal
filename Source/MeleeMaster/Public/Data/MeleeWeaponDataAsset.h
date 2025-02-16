// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponDataAsset.h"
#include "MeleeWeaponDataAsset.generated.h"


class UWeaponHitPathAsset;
enum class EWeaponFightingStatus : uint8;
enum class EWeaponDirection : uint8;


/**
 * @class UMeleeWeaponDataAsset
 * @brief Data asset class for melee weapons, containing attack and block data.
 * Inherits from UWeaponDataAsset.
 */
UCLASS()
class MELEEMASTER_API UMeleeWeaponDataAsset : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor.
	 */
	UMeleeWeaponDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee")
	FMeleeCombinedData Base;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shield")
	uint8 bHasShield : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shield")
	float ShieldGetTime{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shield")
	float ShieldRemoveTime{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shield",
		meta=(EditCondition="bHasShield", EditConditionHides="bHasShield"))
	FMeleeCombinedData Shield;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Shield")
	float ShieldProjectileBlockPercent{0.5f};
};
