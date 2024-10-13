// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/AdvancedReplicatedObject.h"
#include "AbstractWeapon.generated.h"

class AWeaponVisual;
class UWeaponDataAsset;


/**
 * @brief Abstract base class for weapon management in MeleeMaster plugin.
 * 
 * Provides core functionality for replicating weapon data and handling visual elements.
 */
UCLASS(Abstract)
class MELEEMASTER_API UAbstractWeapon : public UAdvancedReplicatedObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the UAbstractWeapon class.
	 * @param ObjectInitializer Object initializer for setting default values.
	 */
	UAbstractWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	static FName FirstHandSlot;
	static FName SecondHandSlot;

protected:
	/** 
	 * @brief Data asset representing weapon's properties.
	 * @remark Replicated using OnRep_Data.
	 */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Data, BlueprintReadOnly, Category="AbstractWeapon|Replicated")
	UWeaponDataAsset* Data;

	/**
	 * @brief Map of weapon visuals.
	 */
	UPROPERTY(Transient, BlueprintReadOnly, Category="AbstractWeapon|Variables")
	TMap<FName, AWeaponVisual*> Visuals;

protected:
	/**
	 * @brief Called when weapon data is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_Data();

public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	
	/**
	 * @brief Retrieves the weapon data asset.
	 * @return The weapon data asset.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	FORCEINLINE UWeaponDataAsset* GetData() const { return Data; }

	/**
	 * @brief Retrieves and checks the weapon data asset.
	 * @return True is GetData() is valid
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsValidData() const;
	
	/**
	 * @brief Sets the weapon data asset.
	 * @param InData The new data asset to be assigned.
	 * @note Only callable on the authority (server).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void SetData(UWeaponDataAsset* InData);

	/**
	 * @brief Sets the visual components of the weapon.
	 * @param InVisuals Array of weapon visual components.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void SetVisual(const TMap<FName, AWeaponVisual*>& InVisuals);

	/**
	 * @brief Clears visual array
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void ClearVisual();

	/**
	 * @brief Sets a visual actor for the weapon at the specified index.
	 * @param Index The index of the visual component to set.
	 * @param InVisual The new visual component.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void SetVisualActor(FName Index, AWeaponVisual* InVisual);

	/**
	 * @brief Removes a visual actor from the weapon at the specified index.
	 * @param Index The name identifier of the visual component to remove.
	 * @return Visual Weapon actor pointer 
	 */
	virtual AWeaponVisual* RemoveVisualActor(FName Index);

	/**
	 * @brief Retrieves the visual components of the weapon.
	 * @param OutVisual Map that will store the retrieved visual components and its keys.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void GetVisuals(TMap<FName, AWeaponVisual*>& OutVisual);

	/**
	 * @brief Retrieves the visual components of the weapon.
	 * @param OutVisual Array that will store the retrieved visual components.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void GetVisual(TArray<AWeaponVisual*>& OutVisual);

	/**
	 * @brief Retrieves a visual actor for the weapon at the specified index.
	 * @param Index The index of the visual component to retrieve.
	 * @param OutVisual The visual component at the specified index.
	 * @return True if the visual component was successfully retrieved.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual bool GetVisualActor(FName Index, AWeaponVisual*& OutVisual);
};
