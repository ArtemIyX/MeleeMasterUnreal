// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponDataAsset.h"
#include "MeleeWeaponDataAsset.generated.h"


enum class EWeaponDirection : uint8;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponCurveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UCurveFloat> Curve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxTime;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeWeaponCurveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponCurveData Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponCurveData Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponCurveData Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponCurveData Left;

	const FWeaponCurveData& Get(EWeaponDirection InDir);
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
	FMeleeWeaponCurveData Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Melee")
	FMeleeWeaponCurveData Block;
};
