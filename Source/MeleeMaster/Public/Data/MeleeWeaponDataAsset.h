// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponDataAsset.h"
#include "MeleeWeaponDataAsset.generated.h"


class UWeaponHitPathAsset;
enum class EWeaponFightingStatus : uint8;
enum class EWeaponDirection : uint8;

/**
 * @struct FDirectionCameraShakes
 * @brief Struct for defining camera shakes for various directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FDirectionCameraShakes
{
	GENERATED_BODY()

	/**
	* @brief Camera shake for forward direction.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> Forward;

	/**
	 * @brief Camera shake for backward direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> Backward;

	/**
	 * @brief Camera shake for right direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> Right;

	/**
	 * @brief Camera shake for left direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> Left;
};

/**
 * @struct FWeaponCurveData
 * @brief Base struct for handling curve data for weapon-related timings.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponCurveData
{
	GENERATED_BODY()

public:
	FWeaponCurveData(): CurveTime(1.0f)
	{
	}

public:
	/**
	 * @brief Float curve that defines the timeline of weapon actions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UCurveFloat> Curve;

	/**
	 * @brief Duration of the curve.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurveTime;
};

/**
 * @struct FMeleeAttackCurveData
 * @brief Struct representing the timing data for melee attacks.
 * Inherits from FWeaponCurveData.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackCurveData : public FWeaponCurveData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor initializing specific properties.
	 */
	FMeleeAttackCurveData() : bDamageForFullPath(false) {}

public:
	/**
	 * @brief Time required before the attack swing begins.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PreAttackLen{0.2f};

	/**
	 * @brief Time duration of the actual hitting attack phase.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HittingTime{0.2f};

	/**
	 * @brief Cooldown time after the attack ends.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PostAttackLen{0.5f};

	/**
	 * @brief The hit path asset used for the attack.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWeaponHitPathAsset* HitPath{nullptr};

	/**
	 * @brief Base damage dealt by the attack.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BasicDamage{35.0f};

	/**
	 * @brief Whether the damage applies for the full path of the attack.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bDamageForFullPath : 1;

	/**
	 * @brief Type of damage dealt.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	/**
	 * @brief Gets the basic damage of the attack.
	 * @return The base damage value.
	 */
	float GetDamage() const;
};

/**
 * @struct FMeleeBlockCurveData
 * @brief Struct representing the timing data for melee blocks.
 * Inherits from FWeaponCurveData.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockCurveData : public FWeaponCurveData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Duration of the stun time after a successful block.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PostBlockLen{0.2f};
};

/**
 * @struct FMeleeAttackData
 * @brief Struct containing directional attack curve data for different melee attack directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Attack curve data for forward direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Forward;

	/**
	 * @brief Attack curve data for backward direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Backward;

	/**
	 * @brief Attack curve data for right direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Right;

	/**
	 * @brief Attack curve data for left direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Left;

	/**
	 * @brief Duration of the stun caused by the attack.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AttackStunLen{0.3f};

	/**
	 * @brief Camera shakes applied during the attack stun phase.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes AttackStunCameraShakes;

	/**
	 * @brief Returns the attack curve data for a given direction.
	 * @param InDir The direction of the attack.
	 * @return The corresponding attack curve data.
	 */
	const FMeleeAttackCurveData& Get(EWeaponDirection InDir) const;
};

/**
 * @struct FMeleeBlockData
 * @brief Struct containing directional block curve data for different melee block directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockData
{
	GENERATED_BODY()

	FMeleeBlockData();

public:
	/**
	 * @brief Block curve data for forward direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Forward;

	/**
	 * @brief Block curve data for backward direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Backward;

	/**
	 * @brief Block curve data for right direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Right;

	/**
	 * @brief Block curve data for left direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Left;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BlockStunLen{0.3f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes BlockRuinShakes;

	/**
	 * @brief Returns the block curve data for a given direction.
	 * @param InDir The direction of the block.
	 * @return The corresponding block curve data.
	 */
	const FMeleeBlockCurveData& Get(EWeaponDirection InDir) const;
};


/**
 * @struct FMeleeCombinedData
 * @brief Struct combining attack and block data for melee weapons.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeCombinedData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor initializing combined data.
	 */
	FMeleeCombinedData();

public:
	/**
	 * @brief Data for melee attack curves and timings.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackData Attack;

	/**
	 * @brief Data for melee parry curves and timings.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackData Parry;

	/**
	 * @brief Data for melee block curves and timings.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockData Block;

	/**
	 * @brief Map of block percentages based on weapon tier.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EWeaponTier, float> BlockPercent;

	/**
	 * @brief Maximum angle for a successful block.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BlockAngle{130.0f};
};

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
};
