#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponTypes.generated.h"

class UWeaponHitPathAsset;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponHitDataElement
{
	GENERATED_BODY()

public:
	FWeaponHitDataElement(): Start(0.0f), End(0.0f)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector End;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FWeaponHitDataElement> Elements;
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponTier : uint8
{
	None = 0 UMETA(Hidden),
	Light = 1,
	Medium = 2,
	High = 3
};

UENUM(Blueprintable, BlueprintType)
enum class EDamageReturn : uint8
{
	Failed,
	Alive,
	Dead
};

/**
 * @struct FDirectionCameraShakes
 * @brief Struct for defining camera shakes for various directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FDirectionCameraShakes
{
	GENERATED_BODY()

public:
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

public:
	TSubclassOf<UCameraShakeBase> Get(EWeaponDirection InWeaponDirection) const;
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
	FMeleeAttackCurveData() : bDamageForFullPath(false)
	{
	}

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

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackDataBase
{
public:
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
	 * @brief Returns the attack curve data for a given direction.
	 * @param InDir The direction of the attack.
	 * @return The corresponding attack curve data.
	 */
	const FMeleeAttackCurveData& Get(EWeaponDirection InDir) const;
};

/**
 * @struct FMeleeAttackData
 * @brief Struct containing directional attack curve data for different melee attack directions.
 */
USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackData : public FMeleeAttackDataBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Camera shakes applied during the starting of charging attack
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes ChargeCameraShakes;

	/**
	 * @brief Camera shakes applied during the first hit (wall, flesh etc)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes HitCameraShakes;

	/**
	 * @brief Camera shakes applied during the starting of hitting process
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes PostChargeCameraShakes;

	/**
	 * @brief Camera shakes applied during the parry stun
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDirectionCameraShakes ParryStun;
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
	FMeleeAttackDataBase Parry;

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

UENUM(Blueprintable, BlueprintType)
enum class EWeaponManagingStatus : uint8
{
	Idle, // No actions, can start to equip another weapon
	Equipping, // Equipping new weapon 
	DeEquipping, // DeEquipping current weapon
	Busy, // We are in fight, using special attack, stunned etc...
	ShieldGetting, // Taking the shield from the back
	ShieldRemoving, // Hiding the shield
	NoWeapon, // No weapon in hands
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponFightingStatus : uint8
{
	Idle, // No actions, ready to fight
	PreAttack, // Clicked lmb
	AttackCharging, // Left click is holding
	BlockCharging, // Right click is holding
	RangeCharging, //  Left click is holding (for Range-Bow)
	Attacking, // Hitting
	PostBlock, // Cooldown after blocked
	PostAttack, // Cooldown after attacked
	HighStunned, // Faced with a maximally charged enemy attack
	BlockStunned, // Blocked, but partially. That's why the block is down
	AttackStunned, // Hit but was blocked, temporarily stunned
	//SuccessfullyPaired, // Successfully sparred and ready to counter-attack
	ParryStunned, // Was sparred, causing him to be slowed and stunned
	Busy, // Jumping, Dashing, Stunned etc..
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponDirection : uint8
{
	Forward,
	Backward,
	Right,
	Left
};

UENUM(Blueprintable, BlueprintType)
enum class EBlockResult : uint8
{
	Parry,
	Block,
	ShieldBlock,
	ShieldProjectileBlock,
	FullDamage,
	Invalid
};


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
	FSequenceData(): FirstPerson(nullptr), ThirdPerson(nullptr)
	{
	}

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


USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAttackAnimMontageData : public FAnimMontageFullData
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
	FAttackAnimMontageData Forward;

	/**
	 * @brief Animation montage data for backward attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAttackAnimMontageData Backward;

	/**
	 * @brief Animation montage data for right attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAttackAnimMontageData Right;

	/**
	 * @brief Animation montage data for left attack direction.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAttackAnimMontageData Left;

	/**
	 * @brief Returns the montage data corresponding to the given direction.
	 * @param InDir The direction for which the montage data is requested.
	 * @return The animation montage data for the specified direction.
	 */
	const FAttackAnimMontageData& Get(EWeaponDirection InDir) const;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackAnimData Parry;

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
	UAimOffsetBlendSpace1D* AimOffset{nullptr};
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FBowArrowData
{
	GENERATED_BODY()

public:
	FBowArrowData();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear)
	TSubclassOf<AActor> VisualActorClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HandSocketTP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HandSocketFP;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeSounds
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag Equip;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag Whoosh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag FleshHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag WallHit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag Parry;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag Block;
};
