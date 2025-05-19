// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WeaponModifierManager.generated.h"

class UAdvancedWeaponManager;
class UAbstractWeapon;

/**
 * @brief Manages weapon modifiers and attack/block states in the MeleeMaster plugin.
 * 
 * Extends AInfo to provide functionality for handling weapon actions and states.
 * This class is Blueprintable and can be used as a type in Blueprints.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API AWeaponModifierManager : public AInfo
{
	GENERATED_BODY()

#pragma region Properties
protected:
	/**
	 * @brief Weak pointer to the advanced weapon manager.
	 * @note Transient and BlueprintReadOnly, not replicated.
	 */
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<UAdvancedWeaponManager> WeaponManagerPtr;
#pragma endregion Properties

#pragma region Client Checks
protected:
	/**
	 * @brief Checks if the current context is a local client.
	 * @return True if the context is a local client, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLocalClient() const;

	/**
	 * @brief Checks if the current context is a third-person client.
	 * @return True if the context is a third-person client, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThirdClient() const;
#pragma endregion Client Checks

#pragma region Weapon Actions
public:
	/**
	 * @brief Event when weapon is charging an attack.
	 * @param InWeapon The weapon performing the attack.
	 * @param InPower The power level of the attack charge.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackCharging(UAbstractWeapon* InWeapon, float InPower);

	/**
	 * @brief Event when weapon executes a melee attack.
	 * @param InWeapon The weapon performing the melee attack.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MeleeAttack(UAbstractWeapon* InWeapon);

	/**
	 * @brief Event when weapon executes a range attack for a weapon.
	 * @param InWeapon The weapon performing the range attack.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RangeAttack(UAbstractWeapon* InWeapon);

	/**
	 * @brief Event when weapon is charging a block.
	 * @param InWeapon The weapon performing the block.
	 * @param InPower The power level of the block charge.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BlockCharging(UAbstractWeapon* InWeapon, float InPower);

	/**
	 * @brief Event when the weapon canceled range attack.
	 * @param CurrentWeapon The weapon whose range attack was canceled.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RangeCanceledAttack(UAbstractWeapon* CurrentWeapon);
#pragma endregion Weapon Actions

#pragma region State Management
public:
	/**
	 * @brief Event when the weapon became Idle
	 * @param CurrentWeapon The current weapon in the idle state.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IdleState(UAbstractWeapon* CurrentWeapon);

	/**
	 * @brief Clears all modifier states.
	 * This function is a BlueprintNativeEvent and BlueprintCallable, allowing it to be overridden and called in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Clear();
#pragma endregion State Management

#pragma region Overrides
public:
	/**
	 * @brief Called when the actor is removed from the world.
	 * @param EndPlayReason The reason the actor is ending play.
	 * Cleans up resources associated with the weapon modifier manager.
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion Overrides

#pragma region Setup
public:
	/**
	 * @brief Sets the advanced weapon manager for this modifier manager.
	 * @param InWeaponManager The weapon manager to set.
	 */
	void SetWeaponManager(UAdvancedWeaponManager* InWeaponManager);
#pragma endregion Setup
};

