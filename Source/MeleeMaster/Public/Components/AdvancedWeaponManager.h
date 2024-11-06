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

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FAnimPlayData
{
	GENERATED_BODY()

public:
	FAnimPlayData();
	FAnimPlayData(UAbstractWeapon* InWeapon, const FAnimMontageFullData& InAnimSet, const float InTime);
	FAnimPlayData(UAbstractWeapon* InWeapon, const FAnimMontageFullData& InAnimSet, const float InTime,
	              const FName& InSectionName);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAbstractWeapon* Weapon{nullptr};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FAnimMontageFullData AnimSet{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Time{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 bUseSection : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SectionName{"None"};
};

USTRUCT(Blueprintable, BlueprintType)
struct FMeleeHitDebugData
{
	GENERATED_BODY()

public:
	FMeleeHitDebugData(): Location(), BaseDamage(0), Multiplier(0)
	{
	}

public:
	FMeleeHitDebugData(const FVector& InLocation, float InBaseDamage, float InMultiplier)
		: Location(InLocation),
		  BaseDamage(InBaseDamage),
		  Multiplier(InMultiplier)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BaseDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Multiplier;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAdvancedWeaponManagerDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvancedWeaponManagerAnimationDelegate,
                                            const FAnimPlayData&, Anim);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponManagerDelegate,
                                            UAbstractWeapon*, InWeaponInstance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWeaponManagerChargingDelegate,
                                               UAbstractWeapon*, InWeaponInstance,
                                               UCurveFloat*, Curve,
                                               float, FinishTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAdvancedWeaponManagerFightingStatusDelegate,
                                             EWeaponFightingStatus, Previous, EWeaponFightingStatus, Current);

/**
 * @class UAdvancedWeaponManager
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
#pragma region Local
	/**
	 * @brief GUID string used to save and identify weapon instances.
	 */
	UPROPERTY(BlueprintReadWrite)
	FString SavedGuid;

	UPROPERTY(Transient)
	TWeakObjectPtr<UAbstractWeapon> NextEquip;

	UPROPERTY(BlueprintReadOnly)
	int32 HitNum{0};

	UPROPERTY(BlueprintReadOnly)
	float HitPower{1.0f};
#pragma endregion

#pragma region Defaults

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="AdvancedWeaponManager|Weapons")
	float MinimalCurveValue{0.1f};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="AdvancedWeaponManager|Weapons")
	bool bDebugMeleeHits{true};

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="AdvancedWeaponManager|Weapons")
	TArray<TSoftObjectPtr<UWeaponDataAsset>> DefaultWeapons;
#pragma endregion

protected:
#pragma region Replicated
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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentCurve)
	UCurveFloat* CurrentCurve;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ChargeStarted)
	float ChargeStarted;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Charge)
	float ChargeWillBeFinished;
#pragma endregion

#pragma region TimerHandles

protected:
	/**
	 * @brief Timer handle for equipping actions.
	 */
	FTimerHandle EquippingTimerHandle;

	FTimerHandle FightTimerHandle;

	FTimerHandle HittingTimerHandle;
#pragma endregion

#pragma region PrivateSet

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

	virtual void SetChargingCurve(UCurveFloat* InCurve);

	virtual void SetChargeFinished(float InFinishTime);

	virtual void SetChargeStarted(float InStartTime);


#pragma endregion

#pragma region ComponentOverride

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


#pragma endregion

#pragma region OnRep

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
	virtual void OnRep_FightingStatus(EWeaponFightingStatus PreviousState);

	/**
	 * @brief Called when the current direction is replicated.
	 */
	UFUNCTION()
	virtual void OnRep_CurrentDirection();

	UFUNCTION()
	virtual void OnRep_CurrentCurve();

	UFUNCTION()
	virtual void OnRep_Charge();

	UFUNCTION()
	virtual void OnRep_ChargeStarted();

#pragma endregion

#pragma region Internal


	virtual void DeEquip_Internal(int32 InIndex);
	virtual void Equip_Internal(int32 InIndex);
	virtual void AddDefaultWeapon_Internal();
#pragma endregion

#pragma region Utils

protected:
	/**
	 * @brief Creates visual representations of the weapon.
	 * @param InAbstractWeapon The weapon for which to create visuals.
	 */
	virtual void CreateVisuals(UAbstractWeapon* InAbstractWeapon);

	virtual void ProcessHits(UAbstractWeapon* InWeapon, const TArray<FHitResult>& InHits);


#pragma endregion

#pragma region Callbacks

protected:
	UFUNCTION()
	virtual void PreAttackFinished();

	UFUNCTION()
	virtual void HitFinished();

	UFUNCTION()
	virtual void MeleeHitProcedure();
	
	UFUNCTION()
	virtual void PostAttackFinished();

	UFUNCTION()
	virtual void PostBlockFinished();

	void EquipNextWeapon_Internal();
	UFUNCTION()
	virtual void DeEquipFinished();
#pragma endregion

#pragma region TryProxy

public:
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

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual void RequestAttackProxy(EWeaponDirection InDirection);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual void RequestAttackReleasedProxy();

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual void RequestBlockProxy(EWeaponDirection InDirection);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual void RequestBlockReleasedProxy();
#pragma endregion

#pragma region Server

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

	UFUNCTION(Server, Reliable)
	void Server_StartAttack(EWeaponDirection InDirection);

	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(Server, Reliable)
	void Server_Block(EWeaponDirection InDirection);

	UFUNCTION(Server, Reliable)
	void Server_UnBlock();

	UFUNCTION(Server, Reliable)
	void Server_Change(int32 InIndex);

#pragma endregion

#pragma region Multi

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_DebugHit(const TArray<FMeleeHitDebugData>& InData);

	/**
	 * @brief Plays an equip animation on all clients.
	 * @param InWeapon The weapon being equipped.
	 * @param InMontageData The animation montage data.
	 * @param MontageTime The duration of the animation (or single section).
	 * @param bUseSection Should start anim montage from specified Section
	 * @param Section Where should start play anim montage from
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayAnim(UAbstractWeapon* InWeapon, const FAnimMontageFullData& InMontageData, float MontageTime,
	                    bool bUseSection = false,
	                    const FName& Section = "Section");

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_CancelCurrentAnim();

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

	UFUNCTION(NetMulticast, Reliable)
	void Multi_DropWeaponVisual(const FString& InWeaponGuid);

#pragma endregion

#pragma region Attach

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
#pragma endregion

#pragma region Exposed

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Misc")
	virtual void ClearBeforeDestroy();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Misc")
	virtual void StopWork();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Weapon")
	virtual void DropWeaponVisual(const FString& InWeaponGuid);

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
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction), Category="AdvancedWeaponManager|Weapon")
	virtual bool RemoveWeapon(int32 InIndex);

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
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual bool CanEquip(int32 InIndex) const;

	/**
	 * @brief Determines if a weapon can be changed based on its index.
	 * @param InIndex The index of the weapon to check.
	 * @return Whether the weapon can be changed.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual bool CanChange(int32 InIndex);

	/**
	 * @brief Determines if a weapon can be de-equipped based on its index.
	 * @param InIndex The index of the weapon to check.
	 * @return Whether the weapon can be de-equipped.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual bool CanDeEquip(int32 InIndex) const;

	// Idle -> PreAttack
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual bool CanStartAttack() const;

	// Charge -> Hit
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual bool CanAttack() const;

	// Idle -> Block
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual bool CanBlock();

	// Block -> Idle
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Fight")
	virtual bool CanUnBlock();
#pragma endregion

#pragma region Getters


	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Charge")
	FORCEINLINE UCurveFloat* GetChargingCurve() const { return CurrentCurve; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Charge")
	FORCEINLINE float GetChargingStartTime() const { return ChargeStarted; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Charge")
	FORCEINLINE float GetChargingFinishTime() const { return ChargeWillBeFinished; }

	/**
	 * @brief Retrieves a weapon by its index.
	 * @param InIndex The index of the weapon to retrieve.
	 * @return The weapon at the specified index.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual UAbstractWeapon* Weapon(int32 InIndex) const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 WeaponIndex(UAbstractWeapon* InWeapon);

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
	 * @brief Retrieves the index of the currently equipped weapon.
	 * @return The index of the current weapon.
	 */
	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 GetCurrentWeaponIndex() const;

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

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Getters")
	float EvaluateCurrentCurve() const;
#pragma endregion

#pragma region Events
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

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FWeaponManagerChargingDelegate OnStartedCharging;

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FWeaponManagerChargingDelegate OnStartedChargingBlock;

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerDelegate OnCancelCurrentTpAnim;

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerDelegate OnCancelCurrentFpAnim;

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerFightingStatusDelegate OnClientFightingStatusChanged;
#pragma endregion
};
