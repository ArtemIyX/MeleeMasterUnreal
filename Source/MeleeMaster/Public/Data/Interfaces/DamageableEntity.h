// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "UObject/Interface.h"
#include "DamageableEntity.generated.h"


// This class does not need to be modified.
UINTERFACE()
class MELEEMASTER_API UDamageableEntity : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEEMASTER_API IDamageableEntity
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveWeaponDamage(AActor* Causer, float Amount, const FHitResult& HitResult,
		TSubclassOf<UDamageType> DamageType,
		EDamageReturn& OutDamageReturn,
		float& OutDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InflictedDamage(AActor* Damaged, EDamageReturn DamageReturn,
		float InTotalDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsAlive() const;
};
