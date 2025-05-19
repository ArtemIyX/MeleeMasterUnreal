// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "MeleeWeapon.generated.h"

struct FMeleeCombinedData;
class UMeleeWeaponDataAsset;
/**
 * @brief Abstract base class for melee weapons in the MeleeMaster plugin.
 * 
 * Extends UAbstractWeapon to provide functionality for managing shields and melee-specific data.
 * This class is Blueprintable, abstract, and can be used as a type in Blueprints.
 */
UCLASS(Blueprintable, Abstract, BlueprintType)
class MELEEMASTER_API UMeleeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()

#pragma region Constructor
public:
	/**
	 * @brief Constructor for the UMeleeWeapon class.
	 * @param ObjectInitializer Object initializer for setting default values.
	 */
	UMeleeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
#pragma endregion Constructor

#pragma region Properties
protected:
	/**
	 * @brief Indicates whether a shield is equipped.
	 * @note Replicated using OnRep_ShieldEquipped.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldEquipped)
	uint8 bShieldEquipped;
#pragma endregion Properties

#pragma region Replication
protected:
	/**
	 * @brief Called when the shield equipped state is replicated.
	 */
	UFUNCTION()
	void OnRep_ShieldEquipped();

public:
	/**
	 * @brief Retrieves the lifetime replicated properties for this class.
	 * @param OutLifetimeProps Array to store the replicated properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
#pragma endregion Replication

#pragma region Shield Management
public:
	/**
	 * @brief Sets whether a shield is equipped.
	 * @param bInValue True to equip a shield, false otherwise.
	 */
	void SetShieldEquipped(bool bInValue);

	/**
	 * @brief Checks if the weapon supports equipping a shield.
	 * @return True if a shield is supported, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponShieldSupported() const;

	/**
	 * @brief Checks if a shield is currently equipped.
	 * @return True if a shield is equipped, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsShieldEquipped() const;
#pragma endregion Shield Management

#pragma region Data
public:
	/**
	 * @brief Retrieves the melee weapon data asset.
	 * @return The melee weapon data asset.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMeleeWeaponDataAsset* GetMeleeData() const;

	/**
	 * @brief Retrieves the current combined melee data.
	 * @return Reference to the current combined melee data.
	 */
	const FMeleeCombinedData& GetCurrentMeleeCombinedData() const;
#pragma endregion Data
};

