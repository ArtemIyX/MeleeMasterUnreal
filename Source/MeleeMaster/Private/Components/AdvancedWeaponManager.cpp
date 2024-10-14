// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/AdvancedWeaponManager.h"

#include "MeleeMaster.h"
#include "Actors/WeaponVisual.h"
#include "Data/WeaponAnimationDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Data/Interfaces/WeaponManagerOwner.h"
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
	ManagingStatus = EWeaponManagingStatus::NoWeapon;
	FightingStatus = EWeaponFightingStatus::Idle;
	CurrentDirection = EWeaponDirection::Forward;
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

void UAdvancedWeaponManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(EquippingTimerHandle);
	Super::EndPlay(EndPlayReason);
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

	UAbstractWeapon* weaponInstance = NewObject<UAbstractWeapon>(GetOwner(), InWeaponAsset->WeaponClass, NAME_None,
	                                                             RF_Transient);
	weaponInstance->SetData(InWeaponAsset);
	weaponInstance->SetGuidString(weaponInstance->MakeRandomGuidString());

	int32 index = WeaponList.Add(weaponInstance);
	CreateVisuals(weaponInstance);
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, WeaponList, this);

	return index;
}

void UAdvancedWeaponManager::CreateVisuals(UAbstractWeapon* InAbstractWeapon)
{
	if (!IsValid(InAbstractWeapon))
		return;

	if (!InAbstractWeapon->IsValidData())
		return;

	const UWeaponDataAsset* data = InAbstractWeapon->GetData();
	if (data->Visuals.Num() <= 0)
	{
		return;
	}
	TArray<AWeaponVisual*> actors;
	AActor* owner = GetOwner();
	const FVector ownerLoc = owner->GetActorLocation();
	actors.Reserve(data->Visuals.Num());
	const FString weaponGuid = InAbstractWeapon->GetGUIDString();
	for (TSubclassOf<AWeaponVisual> visualClass : data->Visuals)
	{
		FVector loc = ownerLoc;
		FRotator rot = FRotator::ZeroRotator;
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = owner;
		spawnParameters.Instigator = Cast<APawn>(owner);
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		spawnParameters.ObjectFlags = RF_Transient;
		AWeaponVisual* visualActor = GetWorld()->SpawnActor<AWeaponVisual>(visualClass, loc, rot, spawnParameters);
		if (IsValid(visualActor))
		{
			visualActor->SetGuidString(weaponGuid);
			actors.Add(visualActor);
		}
	}
	InAbstractWeapon->SetVisual(actors);
}


void UAdvancedWeaponManager::Server_Equip_Implementation(int32 InIndex)
{
	if (!CanEquip(InIndex))
		return;

	// Mark status
	SetManagingStatus(EWeaponManagingStatus::Equipping);

	// Set pointer to current weapon
	UAbstractWeapon* weapon = WeaponList[InIndex];
	SetCurrentWeaponPtr(weapon);

	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	auto delegate = FTimerDelegate::CreateLambda([this]()
	{
		SetManagingStatus(EWeaponManagingStatus::Idle);
		SetFightingStatus(EWeaponFightingStatus::Idle);
	});
	GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, data->EquipTime, false);

	if (IsValid(anims))
	{
		Multi_PlayEquipAnim(weapon, anims->Equip, data->EquipTime);
	}
}

void UAdvancedWeaponManager::Multi_PlayEquipAnim_Implementation(
	UAbstractWeapon* InWeapon,
	const FAnimMontageFullData& Equip,
	float EquipTime)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		OnFpAnim.Broadcast(InWeapon, Equip, EquipTime);
	}
	else
	{
		OnTpAnim.Broadcast(InWeapon, Equip, EquipTime);
	}
}

void UAdvancedWeaponManager::AttachBack(AWeaponVisual* InWeaponVisual)
{
	AActor* owner = GetOwner();
	USkeletalMeshComponent* attachComponent;
	if (owner->Implements<UWeaponManagerOwner>())
	{
		attachComponent = IWeaponManagerOwner::Execute_GetAttachComponent(owner);
	}
	else
	{
		TRACEERROR(LogWeapon, "%s of %s doesn't implement UWeaponManagerOwner interface!",
		           *owner->GetFName().ToString(), *owner->GetClass()->GetFName().ToString());
		// Do not call this
		attachComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
	}

	ENetRole role = owner->GetLocalRole();
	bool bIsLocallyControlled = (role == ROLE_AutonomousProxy);
	TRACE(LogWeapon, "Role: %s", *UEnum::GetValueAsName(role).ToString());


	FName backSocket = InWeaponVisual->GetBackSocket();
	InWeaponVisual->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                                  backSocket);
	if (bIsLocallyControlled)
	{
		if (USkeletalMeshComponent* visualSkeletalMesh = InWeaponVisual->GetSkeletalMeshComponent())
		{
			visualSkeletalMesh->CastShadow = false;
			visualSkeletalMesh->bCastHiddenShadow = false;
			visualSkeletalMesh->SetVisibility(false);
			visualSkeletalMesh->SetHiddenInGame(true);
			InWeaponVisual->SetHidden(true);
		}
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

UAbstractWeapon* UAdvancedWeaponManager::Weapon(int32 InIndex) const
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return nullptr;
	}
	return WeaponList[InIndex];
}

UAbstractWeapon* UAdvancedWeaponManager::WeaponByGuid(FString InGuid)
{
	for (UAbstractWeapon* el : WeaponList)
	{
		if (IsValid(el) && el->GetGUIDString().Equals(InGuid))
		{
			return el;
		}
	}
	return nullptr;
}

int32 UAdvancedWeaponManager::WeaponNum() const
{
	return WeaponList.Num();
}

bool UAdvancedWeaponManager::IsValidWeaponIndex(int32 Index) const
{
	return WeaponList.IsValidIndex(Index);
}

bool UAdvancedWeaponManager::CanEquip(int32 InIndex) const
{
	if (!IsValidWeaponIndex(InIndex))
		return false;

	bool bIdle = ManagingStatus == EWeaponManagingStatus::Idle;
	bool bNoWeapon = ManagingStatus == EWeaponManagingStatus::NoWeapon;
	// Idle or NoWeapon
	if (!(bIdle || bNoWeapon))
	{
		return false;
	}

	// Equip is allowed if current weapon is null
	UAbstractWeapon* currentWeapon = GetCurrentWeapon();
	if (!IsValid(currentWeapon))
	{
		return true;
	}

	// Already equipped
	UAbstractWeapon* wpn = Weapon(InIndex);
	if (wpn == currentWeapon)
	{
		return false;
	}

	return true;
}

void UAdvancedWeaponManager::TryEquipProxy(int32 InIndex)
{
	if (!CanEquip(InIndex))
		return;

	Server_Equip(InIndex);
}
