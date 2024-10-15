// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/WeaponAnimationDataAsset.h"
#include "AdvancedWeaponManager.generated.h"


class AWeaponVisual;
class UAbstractWeapon;
class UWeaponDataAsset;

UENUM(Blueprintable, BlueprintType)
enum class EWeaponManagingStatus : uint8
{
	Idle, // No actions, can start to equip another weapon
	Equipping, // Equipping new weapon 
	DeEquipping, // DeEquipping current weapon
	Busy, // We are in fight, using special attack, stunned etc...
	NoWeapon, // No weapon in hands
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponFightingStatus : uint8
{
	Idle, // No actions, ready to fight
	PreAttack, // Clicked lmb
	AttackCharging, // Left click is holding
	BlockCharging, // Right click is holding
	Attacking, // Hitting
	PostBlock, // Cooldown after blocked
	PostAttack, // Cooldown after attacked
	HighStunned, // Faced with a maximally charged enemy attack
	SuccessfullyPaired, // Successfully sparred and ready to counter-attack
	ParryStunned, // Was sparred, causing him to be slowed and stunned
	Busy, // Jumping, Dashing, Stunned etc..
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponDirection : uint8
{
	Forward,
	Backward,
	Right,
	Left
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAdvancedWeaponManagerDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAdvancedWeaponManagerAnimationDelegate,
                                               UAbstractWeapon*, InWeapon,
                                               const FAnimMontageFullData&, AnimSet,
                                               float, Time);

/**
 * @brief Manages advanced weapon systems for characters.
 * Handles weapon equipping, fighting states, and replication for multiplayer.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MELEEMASTER_API UAdvancedWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets default values for this component's properties.
	 */
	UAdvancedWeaponManager();

public:
	/**
	 * @brief GUID string used to save and identify weapon instances.
	 */
	UPROPERTY(BlueprintReadWrite)
	FString SavedGuid;
	

protected:
	/**
	 * @brief Currently equipped weapon.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeapon)
	UAbstractWeapon* CurrentWeapon;

	/**
	 * @brief List of all weapons managed by this component.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WeaponList)
	TArray<UAbstractWeapon*> WeaponList;

	/**
	 * @brief Current managing status of the weapon (e.g., equipping).
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManagingStatus)
	EWeaponManagingStatus ManagingStatus;

	/**
	 * @brief Current fighting status of the weapon (e.g., blocking, attacking).
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FightingStatus)
	EWeaponFightingStatus FightingStatus;

	/**
	 * @brief Current direction of weapon interaction (e.g., block direction).
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentDirection)
	EWeaponDirection CurrentDirection;

protected:
	/**
	 * @brief Timer handle for equipping actions.
	 */
	FTimerHandle EquippingTimerHandle;

public:


protected:
	/**
	 * @brief Sets the currently equipped weapon.
	 * @param InNewWeapon The new weapon to equip.
	 */
	virtual void SetCurrentWeaponPtr(UAbstractWeapon* InNewWeapon);

	/**
	 * @brief Sets the current managing status of the weapon.
	 * @param InStatus The new managing status.
	 */
	virtual void SetManagingStatus(EWeaponManagingStatus InStatus);

	/**
	 * @brief Sets the current fighting status of the weapon.
	 * @param InStatus The new fighting status.
	 */
	virtual void SetFightingStatus(EWeaponFightingStatus InStatus);

	/**
	 * @brief Sets the current direction of weapon interaction.
	 * @param InDirection The new interaction direction.
	 */
	virtual void SetDirection(EWeaponDirection InDirection);

	/**
	 * @brief Sets the saved GUID value.
	 * @param Value The new GUID value.
	 */
	virtual void SetSavedGuid(FString Value);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	/**
	 * @brief Called when the current weapon is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_CurrentWeapon();

	/**
	 * @brief Called when the weapon list is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_WeaponList();

	/**
	 * @brief Called when the managing status is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_ManagingStatus();

	/**
	 * @brief Called when the fighting status is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_FightingStatus();

	/**
	 * @brief Called when the current direction is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_CurrentDirection();

	/**
	 * @brief Called when the saved GUID is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_SavedGuid();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/**
	 * @brief Creates visual representations of the weapon.
	 * @param InAbstractWeapon The weapon for which to create visuals.
	 */
	virtual void CreateVisuals(UAbstractWeapon* InAbstractWeapon);

protected:
	/**
	 * @brief Equips a weapon based on index.
	 * @param InIndex The index of the weapon to equip.
	 */
	UFUNCTION(Server, Reliable)
	void Server_Equip(int32 InIndex);

	/**
	 * @brief De-equips a weapon based on index.
	 * @param InIndex The index of the weapon to de-equip.
	 */
	UFUNCTION(Server, Reliable)
	void Server_DeEquip(int32 InIndex);

	/**
	 * @brief Plays an equip animation on all clients.
	 * @param InWeapon The weapon being equipped.
	 * @param Equip The animation montage data.
	 * @param EquipTime The duration of the equip animation.
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayAnim(UAbstractWeapon* InWeapon, const FAnimMontageFullData& Equip, float EquipTime);

	/**
	 * @brief Attaches the weapon to the character's hand after equipping.
	 * @note Server is skipped
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AttachHand();

	/**
	 * @brief Attaches the weapon to the character's back after de-equipping.
	 * @param InWeaponGuid The GUID of the weapon being attached to the back.
	 * @note Server is skipped
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AttachBack(const FString& InWeaponGuid);

public:
	/**
	 * @brief Attaches the weapon to the character's back.
	 * @param InVisual The visual representation of the weapon to attach.
	 */
	void AttachBack(AWeaponVisual* InVisual);

	/**
	 * @brief Attaches the weapon to the character's hand.
	 * @param InVisual The visual representation of the weapon to attach.
	 */
	void AttachHand(AWeaponVisual* InVisual);

	/**
	 * @brief Attaches the weapon to the character's back based on the weapon's GUID and visual index.
	 * @param WeaponGuid The GUID of the weapon.
	 * @param VisualIndex The index of the weapon visual.
	 */
	void AttachBack(const FString& WeaponGuid, int32 VisualIndex);

	/**
	 * @brief Attaches the weapon to the character's hand based on the weapon's GUID and visual index.
	 * @param WeaponGuid The GUID of the weapon.
	 * @param VisualIndex The index of the weapon visual.
	 */
	void AttachHand(const FString& WeaponGuid, int32 VisualIndex);

	/**
	 * @brief Retrieves the index of the currently equipped weapon.
	 * @return The index of the current weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 GetCurrentWeaponIndex() const;

	/**
	 * @brief Adds a new weapon to the weapon list.
	 * @param InWeaponAsset The asset data for the new weapon.
	 * @return The index of the newly added weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 AddNewWeapon(UWeaponDataAsset* InWeaponAsset);

	/**
	 * @brief Removes a weapon from the weapon list.
	 * @param InIndex The index of the weapon to remove.
	 * @return Whether the weapon was successfully removed.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual bool RemoveWeapon(int32 InIndex);

	/**
	 * @brief Retrieves a weapon by its index.
	 * @param InIndex The index of the weapon to retrieve.
	 * @return The weapon at the specified index.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual UAbstractWeapon* Weapon(int32 InIndex) const;

	/**
	 * @brief Retrieves a weapon by its GUID.
	 * @param InGuid The GUID of the weapon to retrieve.
	 * @return The weapon with the specified GUID.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual UAbstractWeapon* WeaponByGuid(FString InGuid);

	/**
	 * @brief Gets the number of weapons in the weapon list.
	 * @return The number of weapons.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 WeaponNum() const;

	/**
	 * @brief Checks if a weapon index is valid.
	 * @param Index The index to validate.
	 * @return Whether the index is valid.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual bool IsValidWeaponIndex(int32 Index) const;

	/**
	 * @brief Determines if a weapon can be equipped based on its index.
	 * @param InIndex The index of the weapon to check.
	 * @return Whether the weapon can be equipped.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Manage")
	virtual bool CanEquip(int32 InIndex) const;

	/**
	 * @brief Determines if a weapon can be de-equipped based on its index.
	 * @param InIndex The index of the weapon to check.
	 * @return Whether the weapon can be de-equipped.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Manage")
	virtual bool CanDeEquip(int32 InIndex) const;

	/**
	 * @brief Tries to equip a weapon via a proxy method.
	 * @param InIndex The index of the weapon to equip.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual void TryEquipProxy(int32 InIndex);

	/**
	 * @brief Tries to de-equip a weapon via a proxy method.
	 * @param InIndex The index of the weapon to de-equip.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual void TryDeEquipProxy(int32 InIndex);

public:
	/**
	 * @brief Retrieves the currently equipped weapon.
	 * @return The currently equipped weapon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE UAbstractWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	/**
	 * @brief Retrieves the list of weapons.
	 * @return The weapon list.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	TArray<UAbstractWeapon*> GetWeaponList() const { return WeaponList; }

	/**
	 * @brief Retrieves the current managing status.
	 * @return The current managing status.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponManagingStatus GetManagingStatus() const { return ManagingStatus; }

	/**
	 * @brief Retrieves the current fighting status.
	 * @return The current fighting status.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponFightingStatus GetFightingStatus() const { return FightingStatus; }

	/**
	 * @brief Retrieves the current weapon interaction direction.
	 * @return The current interaction direction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponDirection GetCurrentDirection() const { return CurrentDirection; }

	/**
	 * @brief Event triggered for first-person animation.
	 */
	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerAnimationDelegate OnFpAnim;

	/**
	 * @brief Event triggered for third-person animation.
	 */
	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerAnimationDelegate OnTpAnim;
};
