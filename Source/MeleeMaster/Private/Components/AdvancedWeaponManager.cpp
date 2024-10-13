// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/AdvancedWeaponManager.h"

#include "MeleeMaster.h"
#include "Data/WeaponDataAsset.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Objects/AbstractWeapon.h"
#include "Subsystems/LoggerLib.h"


// Sets default values for this component's properties
UAdvancedWeaponManager::UAdvancedWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


void UAdvancedWeaponManager::SetCurrentWeaponPtr(UAbstractWeapon* InNewWeapon)
{
	this->CurrentWeapon = InNewWeapon;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentWeapon, this);
}

void UAdvancedWeaponManager::SetManagingStatus(EWeaponManagingStatus InStatus)
{
	this->ManagingStatus = InStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ManagingStatus, this);
}

void UAdvancedWeaponManager::SetFightingStatus(EWeaponFightingStatus InStatus)
{
	this->FightingStatus = InStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, FightingStatus, this);
}

void UAdvancedWeaponManager::SetDirection(EWeaponDirection InDirection)
{
	this->CurrentDirection = InDirection;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentDirection, this);
}

// Called when the game starts
void UAdvancedWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UAdvancedWeaponManager::OnRep_CurrentWeapon()
{
}

void UAdvancedWeaponManager::OnRep_WeaponList()
{
}

void UAdvancedWeaponManager::OnRep_ManagingStatus()
{
}

void UAdvancedWeaponManager::OnRep_FightingStatus()
{
}

void UAdvancedWeaponManager::OnRep_CurrentDirection()
{
}


// Called every frame
void UAdvancedWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAdvancedWeaponManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (IsValid(CurrentWeapon))
	{
		sup |= Channel->ReplicateSubobject(CurrentWeapon, *Bunch, *RepFlags);
		sup |= CurrentWeapon->ReplicateSubobjects(Channel, Bunch, RepFlags);
	}
	if (WeaponList.Num() > 0)
	{
		for (UAbstractWeapon* el : WeaponList)
		{
			if (IsValid(el))
			{
				sup |= Channel->ReplicateSubobject(el, *Bunch, *RepFlags);
				sup |= el->ReplicateSubobjects(Channel, Bunch, RepFlags);
			}
		}
	}
	return sup;
}

void UAdvancedWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentWeapon, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, WeaponList, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, ManagingStatus, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, FightingStatus, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentWeapon, Params);
}


bool UAdvancedWeaponManager::CanStartEquippingWeapon(int32 InWeaponIndex) const
{
	return false;
}

int32 UAdvancedWeaponManager::GetCurrentWeaponIndex() const
{
	return INDEX_NONE;
}

int32 UAdvancedWeaponManager::AddNewWeapon(UWeaponDataAsset* InWeaponAsset)
{
	if (!IsValid(InWeaponAsset))
		return INDEX_NONE;

	if (!InWeaponAsset->IsValidToCreate())
		return INDEX_NONE;

	UAbstractWeapon* weaponInstance = NewObject<UAbstractWeapon>(GetOwner(), InWeaponAsset->WeaponClass,
	                                                             MakeUniqueObjectName(
		                                                             GetOwner(), InWeaponAsset->WeaponClass,
		                                                             FName(TEXT("WeaponIntance"))));
	weaponInstance->SetData(InWeaponAsset);


	int32 index = WeaponList.Add(weaponInstance);
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, WeaponList, this);

	Multi_CreateVisual(index);
	return index;
}

void UAdvancedWeaponManager::CreateWeaponVisualsForWeapon(UAbstractWeapon* InAbstractWeapon)
{
	if (!IsValid(InAbstractWeapon))
		return;

	if (!InAbstractWeapon->IsValidData())
		return;
}

void UAdvancedWeaponManager::Multi_CreateVisual_Implementation(int32 InWeaponIndex)
{
	if (WeaponList.IsValidIndex(InWeaponIndex) && IsValid(WeaponList[InWeaponIndex]))
	{
		TRACE(LogWeapon, "Weapon visual created %d: %s", InWeaponIndex,
		      *WeaponList[InWeaponIndex]->GetFName().ToString());
	}
	else
	{
		TRACE(LogWeapon, "Weapon visual not created %d: null", InWeaponIndex)
	}
}

bool UAdvancedWeaponManager::RemoveWeapon(int32 InIndex)
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return false;
	}
	return true;
}

UAbstractWeapon* UAdvancedWeaponManager::Weapon(int32 InIndex)
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return nullptr;
	}
	return WeaponList[InIndex];
}

int32 UAdvancedWeaponManager::WeaponNum() const
{
	return WeaponList.Num();
}
