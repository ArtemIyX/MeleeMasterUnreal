// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponDataAsset.h"
#include "MeleeWeaponDataAsset.generated.h"


enum class EWeaponFightingStatus : uint8;
enum class EWeaponDirection : uint8;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponCurveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UCurveFloat> Curve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurveTime;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackCurveData : public FWeaponCurveData
{
	GENERATED_BODY()

public:
	// Time required to start the swing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PreAttackLen{0.2f};

	// The time of the attack itself. After completion the status will be set to post-attack
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HittingTime{0.2f};

	// Attack cooldown time
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PostAttackLen{0.5f};

	//TODO: Hit attack path asset
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockCurveData : public FWeaponCurveData
{
	GENERATED_BODY()

public:
	// Time of the sun after successful blocking
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PostBlockLen{0.2f};
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeAttackCurveData Left;

	const FMeleeAttackCurveData& Get(EWeaponDirection InDir);
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeBlockData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMeleeBlockCurveData Left;

	const FMeleeBlockCurveData& Get(EWeaponDirection InDir);
};

/**
 * 
 */
UCLASS()
class MELEEMASTER_API UMeleeWeaponDataAsset : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	UMeleeWeaponDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee")
	FMeleeAttackData Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee")
	FMeleeBlockData Block;
};
