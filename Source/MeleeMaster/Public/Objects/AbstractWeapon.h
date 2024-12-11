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
	 * @brief Array of weapon visuals.
	 */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Visuals, BlueprintReadOnly, Category="AbstractWeapon|Variables")
	TArray<AWeaponVisual*> Visuals;

	/**
	 * @brief Unique id for multiplayer replication
	 */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Guid, BlueprintReadOnly, Category="AbstractWeapon|Variables")
	FString Guid;

protected:
	/**
	 * @brief Called when weapon data is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_Data();

	/**
	* @brief Called when weapon visuals is replicated.
	*/
	UFUNCTION()
	virtual void OnRep_Visuals();

	/**
	* @brief Called when Guid variable is replicated.
	*/
	UFUNCTION()
	virtual void OnRep_Guid();

public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:

	UFUNCTION(BlueprintNativeEvent)
	bool IsLeftHandIkRequired() const;

	UFUNCTION(BlueprintNativeEvent)
	FTransform GetLeftHandLocation(ACharacter* InCharacterOwner) const;

	UFUNCTION(BlueprintNativeEvent)
	bool IsRightHandIkRequired() const;

	UFUNCTION(BlueprintNativeEvent)
	FTransform GetRightHandLocation(ACharacter* InCharacterOwner) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsAttackDirected() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsBlockAllowed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsBlockDirected() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	FORCEINLINE FString GetGUIDString() const { return Guid; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void SetGuidString(FString InGuid);

	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual FString MakeRandomGuidString();
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
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void SetVisual(const TArray<AWeaponVisual*>& InVisuals);

	/**
	 * @brief Clears visual array
	 * @note Only references will be cleared. Actors will be still alive
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void ClearVisual();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void DestroyVisuals();

	/**
	 * @brief Sets a visual actor for the weapon at the specified index.
	 * @param InIndex The index of the visual component to set.
	 * @param InVisual The new visual component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void SetVisualActor(int32 InIndex, AWeaponVisual* InVisual);

	/**
	 * @brief Removes a visual actor from the weapon at the specified index.
	 * @param Index The name identifier of the visual component to remove.
	 * @return Visual Weapon actor pointer 
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual AWeaponVisual* RemoveVisualActor(int32 Index);

	/**
	 * @brief Retrieves the visual components of the weapon.
	 * @param OutVisual Array that will store the retrieved visual components.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual void GetVisual(TArray<AWeaponVisual*>& OutVisual) const;

	/**
	 * @brief Retrieves a visual actor for the weapon at the specified index.
	 * @param Index The index of the visual component to retrieve.
	 * @param OutVisual The visual component at the specified index.
	 * @return True if the visual component was successfully retrieved.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual bool GetVisualActor(int32 Index, AWeaponVisual*& OutVisual) const;

	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual int32 GetVisualIndex(const AWeaponVisual* InVisual) const;
	/**
	 * @brief Retrieves a number of visuals for this weapon
	 * @return Visuals.Num()
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual int32 VisualNum() const { return Visuals.Num(); }
};
