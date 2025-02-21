// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimationDataAsset.h"
#include "WeaponTypes.h"
#include "MeleeWeaponAnimDataAsset.generated.h"



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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee")
	FMeleeAttackAnimData Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee")
	FMeleeAttackAnimData Parry;
	/**
	 * @brief Animation montage data for melee blocks in all directions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee")
	FMeleeBlockAnimData Block;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee")
	FMeleeBlockRuinAnimData BlockRuin;
	
	/**
	 * @brief Animation montage data for shield actions, including equip, de-equip, attack, and block.
	 * 
	 * This property is used if the weapon includes a shield, storing the animation montages for shield-related actions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Shield", meta=(ShowOnlyInnerProperties))
	FMeleeShieldAnimData Shield;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Sounds")
	FMeleeSounds SoundPack;
};
