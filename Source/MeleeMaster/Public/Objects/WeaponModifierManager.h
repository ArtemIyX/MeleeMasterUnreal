// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponModifierManager.generated.h"

class UAdvancedWeaponManager;
class UAbstractWeapon;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API AWeaponModifierManager : public AInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<UAdvancedWeaponManager> WeaponManagerPtr;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLocalClient() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThirdClient() const;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackCharging(UAbstractWeapon* InWeapon, float InPower);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MeleeAttack(UAbstractWeapon* InWeapon);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RangeAttack(UAbstractWeapon* InWeapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlockCharging(UAbstractWeapon* InWeapon, float InPower);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IdleState(UAbstractWeapon* CurrentWeapon);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetWeaponManager(UAdvancedWeaponManager* InWeaponManager);

};
