// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimationDataAsset.h"
#include "MeleeWeaponAnimDataAsset.generated.h"


enum class EWeaponDirection : uint8;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackAnimMontageData : public FAnimMontageFullData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName="Attack Section")
	FName AttackSection;
};

/**
 * @struct FMeleeAttackAnimData
 * @brief Struct for handling full animation montage data for melee attacks in all directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackAnimData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Animation montage data for forward attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimMontageData Forward;

	/**
	 * @brief Animation montage data for backward attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimMontageData Backward;

	/**
	 * @brief Animation montage data for right attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimMontageData Right;

	/**
	 * @brief Animation montage data for left attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimMontageData Left;

	/**
	 * @brief Returns the montage data corresponding to the given direction.
	 * @param InDir The direction for which the montage data is requested.
	 * @return The animation montage data for the specified direction.
	 */
	const FMeleeAttackAnimMontageData& Get(EWeaponDirection InDir) const;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockAnimMontageData : public FAnimMontageFullData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LiftingTime{0.35f};
};

/**
 * @struct FMeleeAttackAnimData
 * @brief Struct for handling full animation montage data for melee attacks in all directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockAnimData
{
	GENERATED_BODY()

public:
	/**
	 * @brief Animation montage data for forward block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockAnimMontageData Forward;

	/**
	 * @brief Animation montage data for backward block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockAnimMontageData Backward;

	/**
	 * @brief Animation montage data for right block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockAnimMontageData Right;

	/**
	 * @brief Animation montage data for left block direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockAnimMontageData Left;

	/**
	 * @brief Returns the montage data corresponding to the given direction.
	 * @param InDir The direction for which the montage data is requested.
	 * @return The animation montage data for the specified direction.
	 */
	const FMeleeBlockAnimMontageData& Get(EWeaponDirection InDir) const;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockRuinAnimData
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Forward;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Right;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Left;

	const FAnimMontageFullData& Get(EWeaponDirection InDir) const;
};


/**
 * @struct FMeleeShieldAnimData
 * @brief Struct for managing shield-related animation montages, including equipping, de-equipping, attacking, and blocking.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeShieldAnimData
{
	GENERATED_BODY()

public:
	/** 
	 * @brief Animation montage data for equipping the shield. 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Equip;

	/** 
	 * @brief Animation montage data for de-equipping the shield. 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData DeEquip;

	/** 
	 * @brief Animation montage data for obtaining the shield. 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Get;

	/** 
	 * @brief Animation montage data for removing the shield. 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Remove;

	/** 
	 * @brief Animation montage data for shield-based attacks in various directions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimData Attack;

	/** 
	 * @brief Animation montage data for shield-based blocks in various directions.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockAnimData Block;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockRuinAnimData BlockRuin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSequenceData Idle;
	/**
	 * @brief Aim offset blend space used for weapon aiming.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAimOffsetBlendSpace1D* AimOffset;
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee")
	FMeleeAttackAnimData Attack;

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
};
