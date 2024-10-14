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
	NoWeapon, // WTF ??
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponFightingStatus : uint8
{
	Idle, // No actions, ready to fight
	Charging, // Left click is holding
	Blocking, // Right click is holding
	PostBlock, // Cooldown after blocked
	PostAttack, // Cooldown after attacked
	HighStunned, // Faced with a maximally charged enemy attack
	SuccessfullyPaired, // Successfully sparred and ready to counter attack
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


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MELEEMASTER_API UAdvancedWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAdvancedWeaponManager();
public:
	UPROPERTY(BlueprintReadWrite)
	FString SavedGuid;
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeapon)
	UAbstractWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WeaponList)
	TArray<UAbstractWeapon*> WeaponList;

	// Equipping status
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManagingStatus)
	EWeaponManagingStatus ManagingStatus;

	// Block, Attack etc..
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FightingStatus)
	EWeaponFightingStatus FightingStatus;

	// Block, attack and parry direction
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentDirection)
	EWeaponDirection CurrentDirection;

protected:
	FTimerHandle EquippingTimerHandle;

public:


protected:
	virtual void SetCurrentWeaponPtr(UAbstractWeapon* InNewWeapon);
	virtual void SetManagingStatus(EWeaponManagingStatus InStatus);
	virtual void SetFightingStatus(EWeaponFightingStatus InStatus);
	virtual void SetDirection(EWeaponDirection InDirection);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	UFUNCTION()
	virtual void OnRep_CurrentWeapon();

	UFUNCTION()
	virtual void OnRep_WeaponList();

	UFUNCTION()
	virtual void OnRep_ManagingStatus();

	UFUNCTION()
	virtual void OnRep_FightingStatus();

	UFUNCTION()
	virtual void OnRep_CurrentDirection();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

protected:
	virtual void CreateVisuals(UAbstractWeapon* InAbstractWeapon);

protected:
	UFUNCTION(Server, Reliable)
	void Server_Equip(int32 InIndex);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayEquipAnim(UAbstractWeapon* InWeapon, const FAnimMontageFullData& Equip, float EquipTime);

	// Should be called after Equipped finished
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AttachHand();
public:

	void AttachBack(AWeaponVisual* InVisual);
	void AttachHand(AWeaponVisual* InVisual);
	
	void AttachBack(const FString& WeaponGuid, int32 VisualIndex);
	void AttachHand(const FString& WeaponGuid, int32 VisualIndex);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual bool CanStartEquippingWeapon(int32 InWeaponIndex) const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 GetCurrentWeaponIndex() const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 AddNewWeapon(UWeaponDataAsset* InWeaponAsset);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual bool RemoveWeapon(int32 InIndex);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual UAbstractWeapon* Weapon(int32 InIndex) const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual UAbstractWeapon* WeaponByGuid(FString InGuid);

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual int32 WeaponNum() const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Weapon")
	virtual bool IsValidWeaponIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AdvancedWeaponManager|Manage")
	virtual bool CanEquip(int32 InIndex) const;

	UFUNCTION(BlueprintCallable, Category="AdvancedWeaponManager|Manage")
	virtual void TryEquipProxy(int32 InIndex);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")	FORCEINLINE UAbstractWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	TArray<UAbstractWeapon*> GetWeaponList() const { return WeaponList; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponManagingStatus GetManagingStatus() const { return ManagingStatus; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponFightingStatus GetFightingStatus() const { return FightingStatus; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AdvancedWeaponManager|Getters")
	FORCEINLINE EWeaponDirection GetCurrentDirection() const { return CurrentDirection; }

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerAnimationDelegate OnFpAnim;

	UPROPERTY(BlueprintAssignable, Category="AdvancedWeaponManager|Events")
	FAdvancedWeaponManagerAnimationDelegate OnTpAnim;
};
