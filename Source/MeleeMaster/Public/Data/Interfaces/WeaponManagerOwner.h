// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "UObject/Interface.h"
#include "WeaponManagerOwner.generated.h"

// This class does not need to be modified.
UINTERFACE()
class MELEEMASTER_API UWeaponManagerOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MELEEMASTER_API IWeaponManagerOwner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WeaponManagerOwner")
	USkeletalMeshComponent* GetAttachComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="WeaponManagerOwner")
	EDamageReturn ApplyDamage(AActor* Causer, float Amount,
	                          const FHitResult& HitResult,
	                          TSubclassOf<UDamageType> DamageType);
};
