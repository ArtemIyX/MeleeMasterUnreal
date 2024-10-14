// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/AdvancedDataAsset.h"
#include "WeaponDataAsset.generated.h"

class UWeaponAnimationDataAsset;
class AWeaponVisual;
class UAbstractWeapon;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UWeaponDataAsset : public UAdvancedDataAsset
{
	GENERATED_BODY()

public:
	UWeaponDataAsset();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Main")
	TSubclassOf<UAbstractWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visual")
	TArray<TSubclassOf<AWeaponVisual>> Visuals;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visual")
	UWeaponAnimationDataAsset* Animations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Server|Timing", meta=(ClampMin="0.01"))
	float EquipTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Server|Timing", meta=(ClampMin="0.01"))
	float DeEquipTime;
public:
	bool IsValidToCreate() const;
};
