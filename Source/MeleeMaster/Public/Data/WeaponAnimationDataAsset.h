// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponAnimationDataAsset.generated.h"

class UAimOffsetBlendSpace1D;

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAnimMontageSingleData
{
	GENERATED_BODY()

public:
	FAnimMontageSingleData(): Value(nullptr), Length(1.0f)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UAnimMontage> Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	float Length;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAnimMontageFullData
{
	GENERATED_BODY()

public:
	FAnimMontageFullData()
	{
		FirstPerson = FAnimMontageSingleData();
		ThirdPerson = FAnimMontageSingleData();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageSingleData FirstPerson;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageSingleData ThirdPerson;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FSequenceData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSequence* FirstPerson;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSequence* ThirdPerson;
};


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponAnimationDataAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	UWeaponAnimationDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Manage")
	FAnimMontageFullData Equip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Manage")
	FAnimMontageFullData DeEquip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Misc")
	FSequenceData Idle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Montages|Misc")
	UAimOffsetBlendSpace1D* AimOffset;
};
