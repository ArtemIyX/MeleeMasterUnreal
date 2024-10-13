// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract, BlueprintType)
class MELEEMASTER_API UMeleeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()

public:
	UMeleeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	
};
