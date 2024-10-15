// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAnimationDataAsset.h"
#include "MeleeWeaponAnimDataAsset.generated.h"


enum class EWeaponDirection : uint8;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FMeleeAnimMontageFullData
{
	GENERATED_BODY()

public:

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Backward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData Left;

	const FAnimMontageFullData& Get(EWeaponDirection InDir);
};

/**
 * 
 */
UCLASS()
class MELEEMASTER_API UMeleeWeaponAnimDataAsset : public UWeaponAnimationDataAsset
{
	GENERATED_BODY()
public:
	UMeleeWeaponAnimDataAsset();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee", meta=(ShowOnlyInnerProperties))
	FMeleeAnimMontageFullData Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Melee", meta=(ShowOnlyInnerProperties))
	FMeleeAnimMontageFullData Block;
};
