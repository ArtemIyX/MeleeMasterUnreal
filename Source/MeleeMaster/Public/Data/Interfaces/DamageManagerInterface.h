﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/PlayerState.h"
#include "UObject/Interface.h"
#include "DamageManagerInterface.generated.h"


// This class does not need to be modified.
UINTERFACE()
class MELEEMASTER_API UDamageManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEEMASTER_API IDamageManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RequestDamage(AActor* Causer,
	                   APlayerState* PlayerInstigator,
	                   AActor* Damaged,
	                   float Amount,
	                   const FHitResult& HitResult,
	                   TSubclassOf<UDamageType> DamageType,
	                   EDamageReturn& OutDamageReturn,
	                   float& OutDamage);
};
