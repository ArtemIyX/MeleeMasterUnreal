// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponHitPathAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponHitPathAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	UWeaponHitPathAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ShowOnlyInnerProperties))
	FWeaponHitData Data;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ClampMin="0.0001"))
	float Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main")
	float ZOffset{-35.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main", meta=(ClampMin="0.0001"))
	TEnumAsByte<ETraceTypeQuery> TraceQuery;
};
