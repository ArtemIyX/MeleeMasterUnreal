// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "LongRangeWeapon.generated.h"

class URangeWeaponDataAsset;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API ULongRangeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	URangeWeaponDataAsset* GetRangeData() const;
};
