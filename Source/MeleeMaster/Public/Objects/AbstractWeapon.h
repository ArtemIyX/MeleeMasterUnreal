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

#pragma region Constructor
public:
	/**
	 * @brief Constructor for the UAbstractWeapon class.
	 * @param ObjectInitializer Object initializer for setting default values.
	 */
	UAbstractWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
#pragma endregion Constructor

#pragma region Static Members
public:
	static FName FirstHandSlot;
	static FName SecondHandSlot;
#pragma endregion Static Members

#pragma region Properties
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
	
	UPROPERTY(Transient)
	TWeakObjectPtr<class UAdvancedWeaponManager> WeaponManagerOwner;
	
	
#pragma endregion Properties

#pragma region Replication
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
#pragma endregion Replication

#pragma region Data
public:
	/**
	 * @brief Retrieves the weapon data asset.
	 * @return The weapon data asset.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon|Data")
	FORCEINLINE UWeaponDataAsset* GetData() const { return Data; }

	/**
	 * @brief Retrieves and checks the weapon data asset.
	 * @return True if GetData() is valid
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon|Data")
	virtual bool IsValidData() const;

	/**
	 * @brief Sets the weapon data asset.
	 * @param InData The new data asset to be assigned.
	 * @note Only callable on the authority (server).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Data")
	virtual void SetData(UWeaponDataAsset* InData);
#pragma endregion Data

#pragma region Visual
public:
	/**
	 * @brief Sets the visual components of the weapon.
	 * @param InVisuals Array of weapon visual components.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Visual")
	virtual void SetVisual(const TArray<AWeaponVisual*>& InVisuals);

	/**
	 * @brief Clears visual array
	 * @note Only references will be cleared. Actors will be still alive
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Visual")
	virtual void ClearVisual();

	/**
	 * @brief Destroys all visual actors associated with the weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Visual")
	virtual void DestroyVisuals();

	/**
	 * @brief Sets a visual actor for the weapon at the specified index.
	 * @param InIndex The index of the visual component to set.
	 * @param InVisual The new visual component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Visual")
	virtual void SetVisualActor(int32 InIndex, AWeaponVisual* InVisual);

	/**
	 * @brief Removes a visual actor from the weapon at the specified index.
	 * @param Index The index of the visual component to remove.
	 * @return Visual Weapon actor pointer 
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon|Visual")
	virtual AWeaponVisual* RemoveVisualActor(int32 Index);

	/**
	 * @brief Retrieves the visual components of the weapon.
	 * @param OutVisual Array that will store the retrieved visual components.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon|Visual")
	virtual void GetVisual(TArray<AWeaponVisual*>& OutVisual) const;

	/**
	 * @brief Retrieves a visual actor for the weapon at the specified index.
	 * @param Index The index of the visual component to retrieve.
	 * @param OutVisual The visual component at the specified index.
	 * @return True if the visual component was successfully retrieved.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon|Visual")
	virtual bool GetVisualActor(int32 Index, AWeaponVisual*& OutVisual) const;

	/**
	 * @brief Retrieves the index of a specific visual actor.
	 * @param InVisual The visual actor to find.
	 * @return The index of the visual actor, or -1 if not found.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon|Visual")
	virtual int32 GetVisualIndex(const AWeaponVisual* InVisual) const;

	/**
	 * @brief Retrieves the number of visuals for this weapon.
	 * @return The number of visual components (Visuals.Num()).
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon|Visual")
	virtual int32 VisualNum() const { return Visuals.Num(); }
#pragma endregion Visual

#pragma region GUID
public:
	
	virtual void SetWeaponManager(const TWeakObjectPtr<UAdvancedWeaponManager>& InValue);
	
	/**
	 * @brief Gets the GUID string associated with this weapon.
	 * @return The GUID string.
	 * This function is BlueprintCallable and marked as FORCEINLINE for performance.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	FORCEINLINE FString GetGUIDString() const { return Guid; }

	/**
	 * @brief Sets the GUID string for this weapon.
	 * @param InGuid The new GUID string to set.
	 * This function is BlueprintCallable and BlueprintAuthorityOnly, meaning it can only be called on the server.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbstractWeapon")
	virtual void SetGuidString(FString InGuid);

	/**
	 * @brief Generates a random GUID string for this weapon.
	 * @return The newly generated random GUID string.
	 * This function is BlueprintCallable, allowing it to be called from Blueprints.
	 */
	UFUNCTION(BlueprintCallable, Category="AbstractWeapon")
	virtual FString MakeRandomGuidString();
#pragma endregion GUID

#pragma region IK
public:
	/**
	 * @brief Checks if inverse kinematics (IK) is required for the left hand.
	 * @return True if left hand IK is required, false otherwise.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool IsLeftHandIkRequired() const;

	/**
	 * @brief Retrieves the transform for the left hand's location.
	 * @param InCharacterOwner The character owning this weapon, used to determine the hand location.
	 * @return The transform representing the left hand's location.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent)
	FTransform GetLeftHandLocation(ACharacter* InCharacterOwner) const;

	/**
	 * @brief Checks if inverse kinematics (IK) is required for the right hand.
	 * @return True if right hand IK is required, false otherwise.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool IsRightHandIkRequired() const;

	/**
	 * @brief Retrieves the transform for the right hand's location.
	 * @param InCharacterOwner The character owning this weapon, used to determine the hand location.
	 * @return The transform representing the right hand's location.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints.
	 */
	UFUNCTION(BlueprintNativeEvent)
	FTransform GetRightHandLocation(ACharacter* InCharacterOwner) const;
#pragma endregion IK

#pragma region Combat
public:
	/**
	 * @brief Determines if the weapon's attack is directed (e.g., left, right, up, down).
	 * @return True if the attack is directed, false otherwise.
	 * @note It uses UWeaponDataAsset* data = GetData()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsAttackDirected() const;

	/**
	 * @brief Checks if blocking is allowed with this weapon.
	 * @return True if blocking is allowedUndo changes, false otherwise.
	 * @note It uses UWeaponDataAsset* data = GetData()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsBlockAllowed() const;

	/**
	 * @brief Determines if the weapon's block is directed (e.g., left, right, up, down).
	 * @return True if the block is directed, false otherwise.
	 * @note It uses UWeaponDataAsset* data = GetData()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbstractWeapon")
	virtual bool IsBlockDirected() const;
#pragma endregion Combat
	
};
