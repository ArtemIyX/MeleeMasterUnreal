// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "MeleeWeapon.generated.h"

class UAdvancedWeaponManager;
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
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldEquipped, Category="MeleeWeapon|Shield")
	uint8 bShieldEquipped : 1;

	/**
	 * @brief Indicates whether a ShieldDurability has been set to 0.0
	 * @note It will be set to false, when ShieldDurability reaches MaxShieldDurability
	 * @see ShieldDurability
	 * @see MaxShieldDurability
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldDropped, Category="MeleeWeapon|Shield")
	uint8 bShieldHasDropped : 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,
		Category="MeleeWeapon|Shield|Regen", meta=(UIMin=0, ClampMin=0))
	float ShieldDecreaseRate{1.0f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,
		Category="MeleeWeapon|Shield|Regen", meta=(UIMin=0, ClampMin=0))
	float ShieldIncreaseRate{1.0f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,
		Category="MeleeWeapon|Shield|Regen", meta=(UIMin=0, ClampMin=0))
	float ShieldDecreaseValue{0.1f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,
		Category="MeleeWeapon|Shield|Regen", meta=(UIMin=0, ClampMin=0))
	float ShieldIncreaseValue{0.1f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_ShieldDurability,
		Category="MeleeWeapon|Shield", meta=(UIMin=0, ClampMin=0))
	float ShieldDurability{1.0f};

	UPROPERTY(Transient)
	FTimerHandle ShieldDurabilityTimerHandle;


#pragma endregion Properties

#pragma region Replication

protected:
	/**
	 * @brief Called when the shield equipped state is replicated.
	 */
	UFUNCTION()
	void OnRep_ShieldEquipped();

	UFUNCTION()
	void OnRep_ShieldDurability();

	UFUNCTION()
	void OnRep_ShieldDropped();

public:
	/**
	 * @brief Retrieves the lifetime replicated properties for this class.
	 * @param OutLifetimeProps Array to store the replicated properties.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#pragma endregion Replication

#pragma region Shield Management

protected:
	void SetShieldDropFlag(bool InFlag);
	UFUNCTION()
	void IncreaseShieldDurability();

	UFUNCTION()
	void DecreaseShieldDurability();

public:
	
	virtual void ObjectEndPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	float ConvertIncomingDamageToShieldDamage(float InDamage);

	UFUNCTION(BlueprintNativeEvent)
	void ProcessShieldDamage(float InDamage);

	void StartDecreasingShield(FTimerManager& InTimerManager);
	void StartIncreasingShield(FTimerManager& InTimerManager);
	/**
	 * @brief Sets whether a shield is equipped.
	 * @param bInValue True to equip a shield, false otherwise.
	 */
	void SetShieldEquipped(bool bInValue);
	void SetShieldDurability(float InValue);

	/**
	 * @brief Checks if the weapon supports equipping a shield.
	 * @return True if a shield is supported, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponShieldSupported() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetShieldDurability() const { return ShieldDurability; }

	/**
	 * @brief Checks if a shield is currently equipped.
	 * @return True if a shield is equipped, false otherwise.
	 * This function is BlueprintCallable and BlueprintPure, meaning it can be called from Blueprints and has no side effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsShieldEquipped() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasShieldDropped() const { return bShieldHasDropped; }
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
