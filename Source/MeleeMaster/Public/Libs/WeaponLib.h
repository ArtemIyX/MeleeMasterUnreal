// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeaponLib.generated.h"

/**
 * 
 */
UCLASS()
class MELEEMASTER_API UWeaponLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponLib")
	static float CalculatePlayRate(float InitialSeconds, float ResultSeconds);
};
