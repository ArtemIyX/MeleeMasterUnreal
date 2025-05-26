// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/AdvancedWeaponManager.h"

#include "MathUtil.h"
#include "MeleeMaster.h"
#include "GameFramework/PlayerState.h"
#include "Actors/WeaponVisual.h"
#include "Data/MeleeWeaponAnimDataAsset.h"
#include "Data/MeleeWeaponDataAsset.h"
#include "Data/RangeWeaponAnimDataAsset.h"
#include "Data/RangeWeaponDataAsset.h"
#include "Data/WeaponAnimationDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Data/WeaponHitPathAsset.h"
#include "Data/Interfaces/DamageableEntity.h"
#include "Data/Interfaces/DamageManagerInterface.h"
#include "Data/Interfaces/WeaponManagerOwner.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Objects/AbstractWeapon.h"
#include "Objects/MeleeWeapon.h"
#include "Subsystems/LoggerLib.h"

#include "Math/UnrealMathUtility.h"
#include "Objects/LongRangeWeapon.h"
#include "Objects/WeaponModifierManager.h"


FAnimPlayData::FAnimPlayData(): bUseSection(false)
{
}

FAnimPlayData::FAnimPlayData(UAbstractWeapon* InWeapon,
                             const FAnimMontageFullData& InAnimSet,
                             const float InTime)
{
	this->Weapon = InWeapon;
	this->AnimSet = InAnimSet;
	this->Time = InTime;
	this->bUseSection = false;
}

FAnimPlayData::FAnimPlayData(UAbstractWeapon* InWeapon,
                             const FAnimMontageFullData& InAnimSet,
                             const float InTime,
                             const FName& InSectionName) : FAnimPlayData(InWeapon, InAnimSet, InTime)

{
	this->bUseSection = true;
	this->SectionName = InSectionName;
}

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
	bHasBlocked = false;
	// ...
}


void UAdvancedWeaponManager::SetCurrentWeaponPtr(UAbstractWeapon* InNewWeapon)
{
	this->CurrentWeapon = InNewWeapon;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentWeapon, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_CurrentWeapon();
	}
}

void UAdvancedWeaponManager::SetHasBlocked(bool bInFlag)
{
	this->bHasBlocked = bInFlag;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, bHasBlocked, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_HasBlocked();
	}
}

void UAdvancedWeaponManager::SetCurrentAttackCombo(float InValue)
{
	this->CurrentAttackComboSum = InValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentAttackComboSum, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_CurrentAttackComboSum();
	}
}

void UAdvancedWeaponManager::SetLastAttackComboSavedSum(float InValue)
{
	this->LastAttackComboSavedSum = InValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, LastAttackComboSavedSum, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_LastAttackComboSavedSum();
	}
}

void UAdvancedWeaponManager::SetAttackComboExpireTime(float InValue)
{
	this->AttackComboExpireTime = InValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, AttackComboExpireTime, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_AttackComboExpireTime();
	}
}


void UAdvancedWeaponManager::SetManagingStatus(EWeaponManagingStatus InStatus)
{
	this->ManagingStatus = InStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ManagingStatus, this);
	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_ManagingStatus();
	}
}

void UAdvancedWeaponManager::SetFightingStatus(EWeaponFightingStatus InStatus)
{
	EWeaponFightingStatus previous = this->FightingStatus;
	this->FightingStatus = InStatus;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, FightingStatus, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_FightingStatus(previous);
	}
}

void UAdvancedWeaponManager::SetDirection(EWeaponDirection InDirection)
{
	this->CurrentDirection = InDirection;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentDirection, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_CurrentDirection();
	}
}

void UAdvancedWeaponManager::SetSavedGuid(FString Value)
{
	this->SavedGuid = Value;
}

void UAdvancedWeaponManager::SetChargingCurve(UCurveFloat* InCurve)
{
	this->CurrentCurve = InCurve;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentCurve, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_CurrentCurve();
	}
}

void UAdvancedWeaponManager::SetChargeFinished(float InFinishTime)
{
	this->ChargeWillBeFinished = InFinishTime;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ChargeWillBeFinished, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_Charge();
	}
}

void UAdvancedWeaponManager::SetChargeStarted(float InStartTime)
{
	this->ChargeStarted = InStartTime;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ChargeStarted, this);

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		OnRep_ChargeStarted();
	}
}


// Called when the game starts
void UAdvancedWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_Standalone)
	{
		SetFightingStatus(EWeaponFightingStatus::Idle);
		SetManagingStatus(EWeaponManagingStatus::Idle);
	}
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

void UAdvancedWeaponManager::OnRep_FightingStatus(EWeaponFightingStatus PreviousState)
{
	OnClientFightingStatusChanged.Broadcast(PreviousState, FightingStatus);
}


void UAdvancedWeaponManager::OnRep_CurrentDirection()
{
	OnClientDirectionChanged.Broadcast(CurrentDirection);
}

void UAdvancedWeaponManager::OnRep_CurrentCurve()
{
}

void UAdvancedWeaponManager::OnRep_Charge()
{
}

void UAdvancedWeaponManager::OnRep_ChargeStarted()
{
}

void UAdvancedWeaponManager::OnRep_HasBlocked()
{
}

void UAdvancedWeaponManager::OnRep_CurrentAttackComboSum()
{
	OnCurrentAttackComboSumChanged.Broadcast(GetCurrentAttackComboSum());
}

void UAdvancedWeaponManager::OnRep_LastAttackComboSavedSum()
{
	OnLastAttackComboSavedSumChanged.Broadcast(GetLastComboSavedSum());
}

void UAdvancedWeaponManager::OnRep_AttackComboExpireTime()
{
	OnAttackComboExpireTimeChanged.Broadcast(GetComboExpireTime());
}


// Called every frame
void UAdvancedWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		UpdateModifierCharging();
	}
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
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentDirection, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentCurve, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, ChargeWillBeFinished, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, ChargeStarted, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, bHasBlocked, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentAttackComboSum, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, LastAttackComboSavedSum, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, AttackComboExpireTime, Params);
}

int32 UAdvancedWeaponManager::GetCurrentWeaponIndex() const
{
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (IsValid(cur))
	{
		for (int32 i = 0; i < WeaponList.Num(); ++i)
		{
			if (cur == WeaponList[i])
			{
				return i;
			}
		}
	}
	return INDEX_NONE;
}

bool UAdvancedWeaponManager::IsCurrentWeaponAttackDirected() const
{
	if (UAbstractWeapon* wpn = GetCurrentWeapon())
	{
		return wpn->IsAttackDirected();
	}
	return false;
}

bool UAdvancedWeaponManager::IsCurrentWeaponBlockAllowed() const
{
	if (UAbstractWeapon* wpn = GetCurrentWeapon())
	{
		return wpn->IsBlockAllowed();
	}
	return false;
}

bool UAdvancedWeaponManager::IsCurrentWeaponBlockDirected() const
{
	if (UAbstractWeapon* wpn = GetCurrentWeapon())
	{
		return wpn->IsBlockDirected();
	}
	return false;
}

float UAdvancedWeaponManager::EvaluateCurrentCurve() const
{
	EWeaponFightingStatus fightStatus = GetFightingStatus();

	if (fightStatus == EWeaponFightingStatus::BlockCharging
		&& bHasBlocked)
	{
		return MinimalCurveValue;
	}

	if (fightStatus == EWeaponFightingStatus::AttackCharging
		|| fightStatus == EWeaponFightingStatus::BlockCharging
		|| fightStatus == EWeaponFightingStatus::RangeCharging)
	{
		if (!IsValid(CurrentCurve))
			return MinimalCurveValue;

		// Current time
		UWorld* world = GetWorld();
		if (!world)
		{
			return MinimalCurveValue;
		}
		AGameStateBase* gs = world->GetGameState();
		if (!gs)
		{
			return MinimalCurveValue;
		}

		const float currentServerTime = gs->GetServerWorldTimeSeconds();
		const float finishTime = GetChargingFinishTime();
		const float startTime = GetChargingStartTime();
		if (currentServerTime <= finishTime)
		{
			// Duration of full curve
			const float duration = FMath::Abs(finishTime - startTime);

			// Time to end curve
			const float secondsLeft = finishTime - currentServerTime;

			// Time from start
			const float curveActualTime = duration - secondsLeft;

			// Eval
			return GetChargingCurve()->GetFloatValue(curveActualTime);
		}
		// Eval last curve item
		return GetChargingCurve()->GetFloatValue(GetChargingCurve()->FloatCurve.GetLastKey().Time);
	}
	return MinimalCurveValue;
}

float UAdvancedWeaponManager::GetCurrentHitPower() const
{
	if (UMeleeWeapon* curWpn = Cast<UMeleeWeapon>(GetCurrentWeapon()))
	{
		if (GetFightingStatus() == EWeaponFightingStatus::Attacking)
		{
			return HitPower;
		}
	}
	return 0.0f;
}

bool UAdvancedWeaponManager::IsAttackComboValid() const
{
	if (UWorld* world = GetWorld())
	{
		if (AGameStateBase* gs = world->GetGameState())
		{
			return gs->GetServerWorldTimeSeconds() < AttackComboExpireTime;
		}
	}
	return false;
}

float UAdvancedWeaponManager::EvaluateAttackComboDamage(float InActualDamage) const
{
	if (!IsAttackComboValid())
	{
		return InActualDamage;
	}
	return InActualDamage + CurrentAttackComboSum * InActualDamage;
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

	const int32 index = WeaponList.Add(weaponInstance);
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

		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			visualActor->HideShadow();
		}
		if (IsValid(visualActor))
		{
			visualActor->SetGuidString(weaponGuid);
			actors.Add(visualActor);
		}
	}
	InAbstractWeapon->SetVisual(actors);

	if (GetWorld()->GetNetMode() != NM_Standalone)
	{
		// Attach to owner (on server)
		for (int32 i = 0; i < actors.Num(); ++i)
		{
			actors[i]->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void UAdvancedWeaponManager::ProcessHits(UAbstractWeapon* InWeapon, const TArray<FHitResult>& InHits)
{
	if (InHits.Num() <= 0)
		return;

	AGameModeBase* gm = GetWorld()->GetAuthGameMode();

	if (!IsValid(gm))
	{
		TRACEERROR(LogWeapon, "Failed to get gamemode from world");
		return;
	}
	if (!gm->Implements<UDamageManager>())
	{
		TRACEERROR(LogWeapon, "Gamemode %s must implement UDamageManager!",
		           *gm->GetClass()->GetFName().ToString());
		return;
	}
	TMap<AActor*, FHitResult> hitMap;

	// Make hitmap

	bool bWasWallHit = false;
	for (const FHitResult& hit : InHits)
	{
		if (AActor* hitActor = hit.GetActor())
		{
			if (hitActor != GetOwner())
			{
				if (!hitMap.Contains(hitActor))
				{
					if (hitActor->Implements<UDamageableEntity>())
					{
						if (IDamageableEntity::Execute_IsAlive(hitActor))
						{
							hitMap.Add(hitActor, hit);
						}
					}
					else
					{
						bWasWallHit = true;
					}
				}
			}
		}
	}


	UWeaponDataAsset* data = InWeapon->GetData();

	TArray<FMeleeHitDebugData> debugArr;

	bool bWasHit = false;
	bool bWasFleshHit = false;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(InWeapon))
	{
		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(meleeWeapon->GetData()->Animations);

		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to process hit",
			           *data->GetClass()->GetFName().ToString());
			return;
		}
		const FMeleeCombinedData& meleeData = meleeWeapon->GetCurrentMeleeCombinedData();
		const FMeleeAttackCurveData& attackData = meleeData.Attack.Get(
			CurrentDirection);

		// Calculate damage with current hit power
		float hitDmg = attackData.GetDamage() * HitPower;
		float estimatedDmg = EvaluateAttackComboDamage(hitDmg);

		for (TTuple<AActor*, FHitResult> el : hitMap)
		{
			if (bDebugMeleeHits)
			{
				debugArr.Add(FMeleeHitDebugData(el.Value.Location, estimatedDmg, HitPower));
			}
			TSubclassOf<UDamageType> dmgType = attackData.DamageType;
			EDamageReturn dmgReturn;
			float totalDmg;
			APawn* pawn = Cast<APawn>(GetOwner());
			APlayerState* ps = pawn->GetController()->GetPlayerState<APlayerState>();

			IDamageManager::Execute_RequestDamage(gm,
			                                      /* AActor* Causer */ pawn,
			                                      /* APlayerState* PlayerInstigator */ ps,
			                                      /* AActor* Damaged */ el.Key,
			                                      /* float Amount */ estimatedDmg,
			                                      /* const FHitResult& HitResul*/ el.Value,
			                                      /* TSubclassOf<UDamageType> DamageType */ dmgType,
			                                      /* EDamageReturn& OutDamageReturn */ dmgReturn,
			                                      /* float& OutDamage */ totalDmg);

			// Update melee combo
			{
				SetLastAttackComboSavedSum(GetLastComboSavedSum() + meleeData.Attack.ComboAddAmount);
			}

			if (meleeAnims)
			{
				OnMeleeFleshHitSound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.FleshHit);
				bWasHit = true;
				bWasFleshHit = true;
			}
		}
		if (bWasWallHit)
		{
			OnMeleeWallHitSound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.WallHit);
			bWasHit = true;
		}

		if (bWasHit)
		{
			if (bWasFleshHit)
			{
				OnMeleeFleshHitCameraShake.Broadcast(meleeWeapon, meleeData.Attack.HitCameraShakes, CurrentDirection);
			}
			else
			{
				OnMeleeWallHitCameraShake.Broadcast(meleeWeapon, meleeData.Attack.HitCameraShakes, CurrentDirection);
			}
		}
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to process hit",
		           *InWeapon->GetClass()->GetFName().ToString());
	}


	if (bDebugMeleeHits && debugArr.Num() > 0)
	{
		Multi_DebugHit(debugArr);
	}
}


void UAdvancedWeaponManager::Multi_DebugHit_Implementation(const TArray<FMeleeHitDebugData>& InData)
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
		return;
	if (bDebugMeleeHits)
	{
		for (const FMeleeHitDebugData& el : InData)
		{
			DrawDebugPoint(GetWorld(), el.Location, 2.0f, FColor::White, false, 4.0f, -1);
			DrawDebugString(GetWorld(), el.Location,
			                FString::Printf(TEXT("%1.f (%.1f x %.1f)"), el.BaseDamage * el.Multiplier, el.BaseDamage,
			                                el.Multiplier),
			                nullptr, FColor::White, 4.0f, true, 1);
		}
	}
}

void UAdvancedWeaponManager::PreAttackFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::AttackCharging);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start charging attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attack = meleeWeapon->GetCurrentMeleeCombinedData().Attack.Get(CurrentDirection);
		float currentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		SetChargeStarted(currentTime);
		SetChargeFinished(currentTime + attack.CurveTime);
		UCurveFloat* curve = attack.Curve.LoadSynchronous();
		SetChargingCurve(curve);
		OnStartedCharging.Broadcast(meleeWeapon, GetChargingCurve(), GetChargingFinishTime());
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to finish pre  attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::RangePreAttackFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::RangeCharging);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	if (ULongRangeWeapon* rangeWeapon = Cast<ULongRangeWeapon>(weapon))
	{
		URangeWeaponDataAsset* rangeWeaponData = Cast<URangeWeaponDataAsset>(data);
		if (!IsValid(rangeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start charging attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FWeaponCurveData& curveData = rangeWeaponData->AttackCurve;
		float currentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		SetChargeStarted(currentTime);
		SetChargeFinished(currentTime + curveData.CurveTime);
		UCurveFloat* curve = curveData.Curve.LoadSynchronous();
		SetChargingCurve(curve);
		OnStartedCharging.Broadcast(rangeWeapon, GetChargingCurve(), GetChargingFinishTime());
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to finish pre  attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::HitFinished()
{
	HitPower = 0.0f;
	GetWorld()->GetTimerManager().ClearTimer(FightTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HittingTimerHandle);

	SetFightingStatus(EWeaponFightingStatus::PostAttack);
	UAbstractWeapon* weapon = GetCurrentWeapon();

	if (!IsValid(weapon))
	{
		SetFightingStatus(EWeaponFightingStatus::Idle);
		SetManagingStatus(EWeaponManagingStatus::Idle);
		return;
	}

	if (IsAttackComboValid())
	{
		float previousAttackCombo = GetLastComboSavedSum();
		SetCurrentAttackCombo(GetCurrentAttackComboSum() + previousAttackCombo);
		SetLastAttackComboSavedSum(0.0f);
	}
	else
	{
		SetCurrentAttackCombo(0.0f); // No combo
		SetLastAttackComboSavedSum(0.0f); // No saved combo
	}


	UWeaponDataAsset* data = weapon->GetData();
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start post attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attack = meleeWeapon->GetCurrentMeleeCombinedData().Attack.Get(CurrentDirection);
		float postAttackTime = attack.PostAttackLen;
		TDelegate<TDelegate<void(), FNotThreadSafeNotCheckedDelegateUserPolicy>::RetValType(),
		          FNotThreadSafeNotCheckedDelegateUserPolicy> delegate = FTimerDelegate::CreateUObject(
			this, &UAdvancedWeaponManager::PostAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, postAttackTime, false);

		Client_HitFinished();
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start post attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::PostAttackFinished()
{
	SetFightingStatus(EWeaponFightingStatus::Idle);
	SetManagingStatus(EWeaponManagingStatus::Idle);
}

void UAdvancedWeaponManager::PostBlockFinished()
{
	SetFightingStatus(EWeaponFightingStatus::Idle);
	SetManagingStatus(EWeaponManagingStatus::Idle);
}

void UAdvancedWeaponManager::EquipFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);

	Multi_UpdateWeaponModifier();
}

void UAdvancedWeaponManager::MeleeHitProcedure()
{
	UAbstractWeapon* weapon = GetCurrentWeapon();
	if (!IsValid(weapon))
		return;

	if (!weapon->IsValidData())
		return;

	UWeaponDataAsset* data = weapon->GetData();

	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to melee attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		AActor* owner = GetOwner();
		if (!IsValid(owner))
		{
			TRACEERROR(LogWeapon, "Invalid owner!");
			return;
		}
		APawn* pawnOwner = Cast<APawn>(owner);
		if (!IsValid(pawnOwner))
		{
			TRACEERROR(LogWeapon, "Owner(%s) must be Pawn",
			           *owner->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attackData = meleeWeapon->GetCurrentMeleeCombinedData().Attack.Get(
			CurrentDirection);
		if (!attackData.HitPath)
		{
			TRACEERROR(LogWeapon, "%s hit path of %s is null",
			           *UEnum::GetValueAsString(CurrentDirection),
			           *meleeWeaponData->GetFName().ToString());
			return;
		}
		UWeaponHitPathAsset* hitPath = attackData.HitPath;
		if (!hitPath->Data.Elements.IsValidIndex(HitNum))
		{
			TRACEWARN(LogWeapon, "Invalid %d index of %s %s",
			          HitNum,
			          *UEnum::GetValueAsString(CurrentDirection),
			          *meleeWeaponData->GetFName().ToString());
			return;
		}
		// Make ignore array
		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(GetOwner());
		TArray<AWeaponVisual*> visual;
		weapon->GetVisual(visual);
		actorsToIgnore.Append(visual);

		// Calculate offsets
		FRotator controlRot = FRotator::ZeroRotator;
		if (IsValid(pawnOwner))
		{
			controlRot = pawnOwner->GetControlRotation();
		}
		controlRot.Pitch = 0.0f;
		controlRot.Add(0.0f, -90.0f, 0.0f);
		FVector ownerLoc = pawnOwner->GetActorLocation();
		ownerLoc.Z += hitPath->ZOffset;

		const FVector localStart = hitPath->Data.Elements[HitNum].Start;
		const FVector startRotated = controlRot.RotateVector(localStart);
		const FVector localEnd = hitPath->Data.Elements[HitNum].End;
		const FVector endRotated = controlRot.RotateVector(localEnd);

		const FVector start = startRotated + ownerLoc;
		const FVector end = endRotated + ownerLoc;
		TArray<FHitResult> hits;
		UKismetSystemLibrary::BoxTraceMulti(GetWorld(), start, end,
		                                    FVector(hitPath->Radius), controlRot,
		                                    hitPath->TraceQuery,
		                                    false, actorsToIgnore,
		                                    bDebugMeleeHits
			                                    ? EDrawDebugTrace::Type::ForDuration
			                                    : EDrawDebugTrace::None,
		                                    hits, false, FLinearColor::Red, FLinearColor::Blue,
		                                    bDebugMeleeHits ? 10.0f : 0.0f);
		if (hits.Num() > 0)
		{
			ProcessHits(meleeWeapon, hits);
		}

		HitNum++;
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to execute melee attack line trace",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_Attack_Implementation()
{
	if (!CanAttack())
		return;

	// Evaluate before settings attacking status
	HitPower = EvaluateCurrentCurve();
	SetFightingStatus(EWeaponFightingStatus::Attacking);
	UAbstractWeapon* weapon = GetCurrentWeapon();

	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		AttackMelee_Internal(meleeWeapon);
	}
	else if (ULongRangeWeapon* rangeWeapon = Cast<ULongRangeWeapon>(weapon))
	{
		AttackRange_Internal(rangeWeapon);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::AttackMelee_Internal(UMeleeWeapon* InMeleeWeapon)
{
	UMeleeWeaponDataAsset* meleeWeaponData = InMeleeWeapon->GetMeleeData();
	if (!IsValid(meleeWeaponData))
	{
		TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to melee attack",
		           *InMeleeWeapon->GetData()->GetClass()->GetFName().ToString());
		return;
	}

	UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(InMeleeWeapon->GetData()->Animations);
	if (!IsValid(meleeWeaponData))
	{
		TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to melee attack",
		           *InMeleeWeapon->GetData()->Animations->GetClass()->GetFName().ToString());
		return;
	}
	const FMeleeCombinedData& currentMeleeData = InMeleeWeapon->GetCurrentMeleeCombinedData();
	const FMeleeAttackCurveData& attackData = currentMeleeData.Attack.Get(
		CurrentDirection);

	if (!attackData.HitPath)
	{
		TRACEERROR(LogWeapon, "%s hit path of %s if invalid",
		           *UEnum::GetValueAsString(CurrentDirection),
		           *meleeWeaponData->GetFName().ToString());
		return;
	}
	UWeaponHitPathAsset* hitPath = attackData.HitPath;

	// Will be called after all elements are line-traced
	auto hitFinishDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::HitFinished);
	GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, hitFinishDelegate, attackData.HittingTime, false);


	if (!IsAttackComboValid())
	{
		SetCurrentAttackCombo(0.0f); // No combo
	}
	SetLastAttackComboSavedSum(0.0f); // No saved combo

	float comboExpireTimeout = currentMeleeData.Attack.ComboTimeOutSeconds;
	// Expire time for combo
	SetAttackComboExpireTime(
		GetWorld()->GetGameState()->GetServerWorldTimeSeconds() + comboExpireTimeout);

	// Looped line-trace method
	HitNum = 0;

	const float frequency = attackData.HittingTime / FMath::Clamp(hitPath->Data.Elements.Num(), 1,
	                                                              TNumericLimits<int32>::Max() - 1);
	auto hittingDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::MeleeHitProcedure);
	GetWorld()->GetTimerManager().SetTimer(HittingTimerHandle, hittingDelegate, frequency, true);

	const FMeleeAttackAnimData& attackAnimData = InMeleeWeapon->IsShieldEquipped()
		                                             ? meleeAnims->Shield.Attack
		                                             : meleeAnims->Attack;
	const FAttackAnimMontageData& attackAnim = attackAnimData.Get(CurrentDirection);

	Multi_PlayAnim(InMeleeWeapon, attackAnim, attackData.HittingTime, true,
	               attackAnim.AttackSection);
	Multi_MeleeChargeFinished();

	OnMeleeWhooshSound.Broadcast(InMeleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.Whoosh);
	OnMeleeAttackCameraShake.Broadcast(InMeleeWeapon, currentMeleeData.Attack.PostChargeCameraShakes, CurrentDirection);
}

void UAdvancedWeaponManager::AttackRange_Internal(ULongRangeWeapon* InRangeWeapon)
{
	GetWorld()->GetTimerManager().ClearTimer(FightTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HittingTimerHandle);

	SetFightingStatus(EWeaponFightingStatus::PostAttack);
	UAbstractWeapon* weapon = GetCurrentWeapon();

	if (!IsValid(weapon))
	{
		SetFightingStatus(EWeaponFightingStatus::Idle);
		SetManagingStatus(EWeaponManagingStatus::Idle);
		return;
	}

	UWeaponDataAsset* data = weapon->GetData();
	/*if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start post attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attack = meleeWeapon->GetCurrentMeleeCombinedData().Attack.Get(CurrentDirection);
		float postAttackTime = attack.PostAttackLen;
		auto delegate = FTimerDelegate::CreateUObject(
			this, &UAdvancedWeaponManager::PostAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, postAttackTime, false);
	}
	else */
	if (ULongRangeWeapon* rangeWeapon = Cast<ULongRangeWeapon>(weapon))
	{
		URangeWeaponDataAsset* rangeData = InRangeWeapon->GetRangeData();
		if (!IsValid(rangeData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to range attack",
			           *InRangeWeapon->GetData()->GetClass()->GetFName().ToString());
			return;
		}

		URangeWeaponAnimDataAsset* rangeAnimData = Cast<
			URangeWeaponAnimDataAsset>(InRangeWeapon->GetData()->Animations);
		if (!IsValid(rangeData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to range attack",
			           *InRangeWeapon->GetData()->Animations->GetClass()->GetFName().ToString());
			return;
		}

		rangeWeapon->FireArrow(HitPower);

		float postAttackTime = rangeData->PostAttackLen;
		auto delegate = FTimerDelegate::CreateUObject(
			this, &UAdvancedWeaponManager::PostAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, postAttackTime, false);

		Multi_PlayAnim(rangeWeapon, rangeAnimData->Pull, postAttackTime, true, rangeAnimData->Pull.AttackSection);
		Multi_RangeChargingFinished();
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start post attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

bool UAdvancedWeaponManager::IsLocalCustomPlayer()
{
	if (APawn* ownerPawn = GetOwner<APawn>())
	{
		// For single player
		if (GetWorld()->GetNetMode() == NM_Standalone)
		{
			// Only player should control local pawn
			return ownerPawn->IsPlayerControlled();
		}
		// For multiplayer only client, only locally controlled and only player controlled
		return ownerPawn->GetLocalRole() == ROLE_AutonomousProxy
			&& ownerPawn->IsLocallyControlled()
			&& ownerPawn->IsPlayerControlled();
	}
	return false;
}


void UAdvancedWeaponManager::StartParry(EWeaponDirection InDirection)
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(HittingTimerHandle);
	timerManager.ClearTimer(FightTimerHandle);
	timerManager.ClearTimer(EquippingTimerHandle);

	// Save direction
	SetDirection(InDirection);
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::AttackCharging);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start melee attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);

		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to start melee attack",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& parry = meleeWeapon->GetCurrentMeleeCombinedData().Parry.Get(CurrentDirection);
		float currentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		SetChargeStarted(currentTime);
		SetChargeFinished(currentTime + parry.CurveTime);

		UCurveFloat* curve = parry.Curve.LoadSynchronous();
		SetChargingCurve(curve);
		OnStartedCharging.Broadcast(meleeWeapon, GetChargingCurve(), GetChargingFinishTime());
		OnParry.Broadcast(meleeWeapon);
		const FMeleeAttackAnimData& parryData = meleeWeapon->IsShieldEquipped()
			                                        ? meleeAnims->Shield.Parry
			                                        : meleeAnims->Parry;
		auto dirParryData = parryData.Get(InDirection);
		Multi_PlayAnim(weapon, dirParryData, parry.PreAttackLen);

		OnMeleeParrySound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.Parry);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_Block_Implementation(EWeaponDirection InDirection)
{
	if (!CanBlock())
		return;
	GetWorld()->GetTimerManager().ClearTimer(FightTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HittingTimerHandle);

	// Reset block flag, that curve value will be evaluated in right way
	SetHasBlocked(false);
	SetDirection(InDirection);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		SetManagingStatus(EWeaponManagingStatus::Busy);
		SetFightingStatus(EWeaponFightingStatus::BlockCharging);

		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to block",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to block",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeBlockCurveData& blockData = meleeWeapon->GetCurrentMeleeCombinedData().Block.Get(CurrentDirection);
		float currentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		SetChargeStarted(currentTime);
		SetChargeFinished(currentTime + blockData.CurveTime);
		UCurveFloat* curve = blockData.Curve.LoadSynchronous();
		SetChargingCurve(curve);
		OnStartedChargingBlock.Broadcast(meleeWeapon, GetChargingCurve(), GetChargingFinishTime());

		const FMeleeBlockAnimData& blockAnimData = meleeWeapon->IsShieldEquipped()
			                                           ? meleeAnims->Shield.Block
			                                           : meleeAnims->Block;
		const FMeleeBlockAnimMontageData& blockAnim = blockAnimData.Get(CurrentDirection);

		Multi_PlayAnim(meleeWeapon, blockAnim, blockAnim.LiftingTime);
	}
	else if (ULongRangeWeapon* rangeWeapon = Cast<ULongRangeWeapon>(weapon))
	{
		SetManagingStatus(EWeaponManagingStatus::Idle);
		SetFightingStatus(EWeaponFightingStatus::Idle);
		Multi_CancelCurrentAnim();
		Multi_RangeCanceled();
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to block",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_UnBlock_Implementation()
{
	if (!CanUnBlock())
		return;

	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::PostBlock);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	//UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start melee attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}
		const FMeleeBlockCurveData& blockData = meleeWeapon->GetCurrentMeleeCombinedData().Block.Get(CurrentDirection);
		auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::PostBlockFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, blockData.PostBlockLen, false);
		Multi_CancelCurrentAnim();
		Client_BlockChargingFinished();
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to unblock",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_Change_Implementation(int32 InIndex)
{
	if (!CanChange(InIndex))
		return;

	if (UAbstractWeapon* wpn = Weapon(InIndex))
	{
		NextEquip = wpn;
		DeEquip_Internal(GetCurrentWeaponIndex());
	}
}

void UAdvancedWeaponManager::Server_GetShield_Implementation()
{
	if (!CanGetShield())
		return;

	SetManagingStatus(EWeaponManagingStatus::ShieldGetting);
	SetFightingStatus(EWeaponFightingStatus::Busy);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to equip shield",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) equip shield",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}
		meleeWeapon->SetShieldEquipped(true);
		auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::ShieldRaiseFinished);

		GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, meleeWeaponData->ShieldGetTime, false);
		Multi_PlayAnim(meleeWeapon, meleeAnims->Shield.Get, meleeWeaponData->ShieldGetTime);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to equip shield",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::ShieldRaiseFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);
}

void UAdvancedWeaponManager::ShieldRemoveFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);
}

void UAdvancedWeaponManager::BlockStunFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);
}

void UAdvancedWeaponManager::AttackStunFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);
}

void UAdvancedWeaponManager::ParryStunFinished()
{
	SetManagingStatus(EWeaponManagingStatus::Idle);
	SetFightingStatus(EWeaponFightingStatus::Idle);
}

void UAdvancedWeaponManager::Server_RemoveShield_Implementation()
{
	if (!CanRemoveShield())
		return;

	SetManagingStatus(EWeaponManagingStatus::ShieldRemoving);
	SetFightingStatus(EWeaponFightingStatus::Busy);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to remove shield",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) remove shield",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}
		meleeWeapon->SetShieldEquipped(false);
		auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::ShieldRemoveFinished);

		GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, meleeWeaponData->ShieldRemoveTime,
		                                       false);
		Multi_PlayAnim(meleeWeapon, meleeAnims->Shield.Remove, meleeWeaponData->ShieldRemoveTime);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to remove shield",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}


void UAdvancedWeaponManager::EquipNextWeapon_Internal()
{
	if (NextEquip.IsValid())
	{
		int32 wpnIndex = WeaponIndex(NextEquip.Get());
		if (wpnIndex != INDEX_NONE)
		{
			if (CanEquip(wpnIndex))
			{
				Equip_Internal(wpnIndex);
				NextEquip.Reset();
			}
		}
	}
}

void UAdvancedWeaponManager::DeEquipFinished()
{
	UAbstractWeapon* curWeapon = GetCurrentWeapon();
	FString guid = curWeapon->GetGUIDString();
	SetSavedGuid(guid);
	Multi_AttachBack(guid);
	SetCurrentWeaponPtr(nullptr);
	SetManagingStatus(EWeaponManagingStatus::NoWeapon);
	SetFightingStatus(EWeaponFightingStatus::Idle);

	if (NextEquip.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
		{
			EquipNextWeapon_Internal();
		}));
	}
}


void UAdvancedWeaponManager::DeEquip_Internal(int32 InIndex)
{
	if (!CanDeEquip(InIndex))
		return;

	// Mark status
	SetManagingStatus(EWeaponManagingStatus::DeEquipping);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::DeEquipFinished);
	GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, data->DeEquipTime, false);

	if (!IsValid(anims))
	{
		return;
	}

	// If melee weapon -> check if shield raised
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		if (UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims))
		{
			const FAnimMontageFullData& deEquipData = meleeWeapon->IsShieldEquipped()
				                                          ? meleeAnims->Shield.DeEquip
				                                          : anims->DeEquip;
			Multi_PlayAnim(weapon, deEquipData, data->DeEquipTime);
			return;
		}
	}

	Multi_PlayAnim(weapon, anims->DeEquip, data->DeEquipTime);
}


void UAdvancedWeaponManager::Server_DeEquip_Implementation(int32 InIndex)
{
	DeEquip_Internal(InIndex);
}


void UAdvancedWeaponManager::Equip_Internal(int32 InIndex)
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

	auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::EquipFinished);
	GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, data->EquipTime, false);

	if (!IsValid(anims))
	{
		return;
	}
	// If melee weapon -> check if shield raised
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		if (UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims))
		{
			const FAnimMontageFullData& equipData = meleeWeapon->IsShieldEquipped()
				                                        ? meleeAnims->Shield.Equip
				                                        : anims->Equip;
			Multi_PlayAnim(weapon, equipData, data->DeEquipTime);

			OnEquipSound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.Equip);
			return;
		}
	}
	Multi_PlayAnim(weapon, anims->Equip, data->EquipTime);
}

void UAdvancedWeaponManager::AddDefaultWeapons()
{
	if (DefaultWeapons.Num() <= 0)
		return;

	TArray<UWeaponDataAsset*> assets;
	assets.Reserve(DefaultWeapons.Num());
	for (TSoftObjectPtr<UWeaponDataAsset> wpn : DefaultWeapons)
	{
		assets.Add(wpn.LoadSynchronous());
	}
	for (UWeaponDataAsset* asset : assets)
	{
		AddNewWeapon(asset);
	}
}


void UAdvancedWeaponManager::Server_Equip_Implementation(int32 InIndex)
{
	Equip_Internal(InIndex);
}

void UAdvancedWeaponManager::Server_StartAttack_Implementation(EWeaponDirection InDirection)
{
	if (!CanStartAttack())
		return;

	// Save direction
	SetDirection(InDirection);

	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::PreAttack);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start melee attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);

		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to start melee attack",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeCombinedData& currentData = meleeWeapon->GetCurrentMeleeCombinedData();
		const FMeleeAttackCurveData& attackData = currentData.Attack.Get(InDirection);

		auto initialDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::PreAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, initialDelegate, attackData.PreAttackLen, false);

		const FMeleeAttackAnimData& attackAnimData = meleeWeapon->IsShieldEquipped()
			                                             ? meleeAnims->Shield.Attack
			                                             : meleeAnims->Attack;
		const FAttackAnimMontageData& attackAnim = attackAnimData.Get(InDirection);
		FAnimMontageFullData montageData = attackAnim;
		Multi_PlayAnim(weapon, montageData, attackData.PreAttackLen);
		OnMeleeChargeCameraShake.Broadcast(meleeWeapon, currentData.Attack.ChargeCameraShakes, InDirection);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_StartAttackSimple_Implementation()
{
	if (!CanStartAttack())
		return;

	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::PreAttack);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		// TODO: Melee undirected attack

		TRACEERROR(LogWeapon, "%s is not NOT IMPLEMENTED for 'UMeleeWeapon'",
		           *weapon->GetClass()->GetFName().ToString());
	}
	else if (ULongRangeWeapon* rangeWeapon = Cast<ULongRangeWeapon>(weapon))
	{
		URangeWeaponDataAsset* rangeData = rangeWeapon->GetRangeData();
		if (!IsValid(rangeData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start range attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		URangeWeaponAnimDataAsset* rangeAnims = Cast<URangeWeaponAnimDataAsset>(anims);
		if (!IsValid(rangeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to start range attack",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		auto initialDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::RangePreAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, initialDelegate, rangeData->PreAttackLen, false);

		const FAttackAnimMontageData& attackAnimData = rangeAnims->Pull;

		Multi_PlayAnim(weapon, attackAnimData, rangeData->PreAttackLen);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start simple attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Multi_PlayAnim_Implementation(
	UAbstractWeapon* InWeapon,
	const FAnimMontageFullData& InAnimSet,
	float InTimeLen,
	bool bUseSection,
	const FName& Section)
{
	SetSavedGuid(InWeapon->GetGUIDString());
	FAnimPlayData data;
	if (bUseSection)
	{
		data = FAnimPlayData(InWeapon, InAnimSet, InTimeLen, Section);
	}
	else
	{
		data = FAnimPlayData(InWeapon, InAnimSet, InTimeLen);
	}

	if (IsLocalCustomPlayer())
	{
		OnFpAnim.Broadcast(data);
	}
	else
	{
		OnTpAnim.Broadcast(data);
	}
}

void UAdvancedWeaponManager::Multi_PlayVisualAnim_Implementation(UAbstractWeapon* InWeapon,
                                                                 const FAnimMontageFullData& InAnimSet, float InTimeLen,
                                                                 int32 VisualIndex, bool bUseSection,
                                                                 const FName& Section)
{
	FAnimPlayData data;
	if (bUseSection)
	{
		data = FAnimPlayData(InWeapon, InAnimSet, InTimeLen, Section);
	}
	else
	{
		data = FAnimPlayData(InWeapon, InAnimSet, InTimeLen);
	}

	AWeaponVisual* wpnVisual;
	bool bVisual = InWeapon->GetVisualActor(VisualIndex, wpnVisual);
	if (bVisual && IsValid(wpnVisual))
	{
		wpnVisual->PlayAnim(data, IsLocalCustomPlayer());
	}
}


void UAdvancedWeaponManager::Multi_AttachHand_Implementation()
{
	// Skip servers, it is already attached to actor
	// Do not skip NM_Standalone
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	UAbstractWeapon* current = GetCurrentWeapon();
	if (!IsValid(current))
		return;
	if (!current->IsValidData())
		return;

	const int32 n = current->VisualNum();
	for (int32 i = 0; i < n; ++i)
	{
		AttachHand(current->GetGUIDString(), n);
	}
}

void UAdvancedWeaponManager::Multi_AttachBack_Implementation(const FString& InWeaponGuid)
{
	// Skip servers, it is already attached to actor
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	UAbstractWeapon* wpn = WeaponByGuid(InWeaponGuid);
	if (!IsValid(wpn))
		return;
	if (!wpn->IsValidData())
		return;

	const int32 n = wpn->VisualNum();
	for (int32 i = 0; i < n; ++i)
	{
		AttachBack(InWeaponGuid, n);
	}
}

void UAdvancedWeaponManager::Multi_CancelCurrentAnim_Implementation()
{
	if (IsLocalCustomPlayer())
	{
		OnCancelCurrentFpAnim.Broadcast();
	}
	else
	{
		OnCancelCurrentTpAnim.Broadcast();
	}
}


void UAdvancedWeaponManager::Multi_DropWeaponVisual_Implementation(const FString& InWeaponGuid)
{
	// Skip server
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (UAbstractWeapon* weapon = WeaponByGuid(InWeaponGuid))
	{
		TArray<AWeaponVisual*> weaponVisuals;
		weapon->GetVisual(weaponVisuals);
		for (AWeaponVisual* visual : weaponVisuals)
		{
			if (IsValid(visual))
			{
				// Manipulate visibility only for local player (camera case)
				if (IsLocalCustomPlayer())
				{
					visual->Show();
				}
				visual->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				visual->ActivatePhysics();
			}
		}
	}
}


void UAdvancedWeaponManager::Client_Blocked_Implementation(EWeaponDirection InDirection,
                                                           const FMeleeBlockData& InBlockData)
{
	OnClientBlocked.Broadcast(InDirection, InBlockData);
}

void UAdvancedWeaponManager::Client_ParryStun_Implementation(EWeaponDirection InDirection,
                                                             const FMeleeAttackData& InAttackData)
{
	OnClientParryStunned.Broadcast(InDirection, InAttackData);
}


void UAdvancedWeaponManager::Client_HitFinished_Implementation()
{
}

void UAdvancedWeaponManager::Multi_MeleeChargeFinished_Implementation()
{
	if (IsValid(CurrentWeapon) && ClientWeaponModifierManager.IsValid())
	{
		ClientWeaponModifierManager->MeleeAttack(CurrentWeapon);
	}
}

void UAdvancedWeaponManager::Multi_RangeChargingFinished_Implementation()
{
	if (IsValid(CurrentWeapon) && ClientWeaponModifierManager.IsValid())
	{
		ClientWeaponModifierManager->RangeAttack(CurrentWeapon);
	}
}

void UAdvancedWeaponManager::Multi_RangeCanceled_Implementation()
{
	if (IsValid(CurrentWeapon) && ClientWeaponModifierManager.IsValid())
	{
		ClientWeaponModifierManager->RangeCanceledAttack(CurrentWeapon);
	}
}

void UAdvancedWeaponManager::Client_BlockChargingFinished_Implementation()
{
}

void UAdvancedWeaponManager::UpdateModifierCharging()
{
	if (IsValid(CurrentWeapon) && ClientWeaponModifierManager.IsValid())
	{
		EWeaponFightingStatus fightStatus = GetFightingStatus();
		if (fightStatus == EWeaponFightingStatus::PreAttack)
		{
			ClientWeaponModifierManager->AttackCharging(CurrentWeapon, MinimalCurveValue);
		}
		else if (fightStatus == EWeaponFightingStatus::AttackCharging
			|| fightStatus == EWeaponFightingStatus::RangeCharging)
		{
			ClientWeaponModifierManager->AttackCharging(CurrentWeapon, EvaluateCurrentCurve());
		}
		else if (fightStatus == EWeaponFightingStatus::BlockCharging)
		{
			ClientWeaponModifierManager->BlockCharging(CurrentWeapon, EvaluateCurrentCurve());
		}
		else
		{
			ClientWeaponModifierManager->IdleState(CurrentWeapon);
		}
	}
}


void UAdvancedWeaponManager::Multi_UpdateWeaponModifier_Implementation()
{
	// Skip server
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (ClientWeaponModifierManager.IsValid())
	{
		ClientWeaponModifierManager->Destroy();
		ClientWeaponModifierManager.Reset();
	}
	if (IsValid(CurrentWeapon))
	{
		if (UWeaponDataAsset* data = CurrentWeapon->GetData())
		{
			TSubclassOf<AWeaponModifierManager> modifierManagerClass = data->VisualModifier;
			if (modifierManagerClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetOwner()->GetInstigator();

				AWeaponModifierManager* modifier = GetWorld()->SpawnActor<AWeaponModifierManager>(
					modifierManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				modifier->SetWeaponManager(this);
				ClientWeaponModifierManager = modifier;
			}
		}
	}
}

void UAdvancedWeaponManager::AttachBack(AWeaponVisual* InVisual)
{
	// Skip server, it is already attached to actor
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (!IsValid(InVisual))
		return;
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

	FName backSocket = InVisual->GetBackSocket();
	InVisual->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                            backSocket);

	if (IsLocalCustomPlayer())
	{
		InVisual->Hide();
	}
}

void UAdvancedWeaponManager::AttachHand(AWeaponVisual* InVisual)
{
	// Skip servers, it is already attached to actor
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (!IsValid(InVisual))
		return;
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

	const FName handSocket = InVisual->GetHandSocket();
	InVisual->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                            handSocket);
	FString wpn = InVisual->GetGUIDString();
	//TRACE(LogWeapon, "Visual '%s' was attached to hand. Saved Guid: '%s'", *wpn, *SavedGuid);

	// Manipulate visibility for local player (camera case)
	if (IsLocalCustomPlayer())
	{
		InVisual->Show();
	}
}

void UAdvancedWeaponManager::AttachBack(const FString& WeaponGuid, int32 VisualIndex)
{
	UAbstractWeapon* weapon = WeaponByGuid(WeaponGuid);
	if (!IsValid(weapon))
		return;
	if (!weapon->IsValidData())
		return;
	AWeaponVisual* visual;
	if (weapon->GetVisualActor(VisualIndex, visual))
	{
		AttachBack(visual);
	}
}


void UAdvancedWeaponManager::AttachHand(const FString& WeaponGuid, int32 InVisualIndex)
{
	UAbstractWeapon* weapon = WeaponByGuid(WeaponGuid);
	if (!IsValid(weapon))
		return;
	if (!weapon->IsValidData())
		return;
	AWeaponVisual* visual;
	if (weapon->GetVisualActor(InVisualIndex, visual))
	{
		AttachHand(visual);
	}
}

void UAdvancedWeaponManager::NotifyPlayWeaponAnim(UAbstractWeapon* InWeapon, const FAnimMontageFullData& InMontageData,
                                                  float MontageTime, int32 VisualIndex, bool bUseSection,
                                                  const FName& Section)
{
	Multi_PlayVisualAnim(InWeapon, InMontageData, MontageTime, VisualIndex, bUseSection, Section);
}

void UAdvancedWeaponManager::NotifyEnemyMeleeBlocked()
{
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::AttackStunned);
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(HittingTimerHandle);
	timerManager.ClearTimer(FightTimerHandle);
	timerManager.ClearTimer(EquippingTimerHandle);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s)to apply attack stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}
		const float attackStunLen = meleeWeapon->GetCurrentMeleeCombinedData().Attack.AttackStunLen;
		timerManager.SetTimer(FightTimerHandle,
		                      FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::AttackStunFinished),
		                      attackStunLen, false);

		//Multi_CancelCurrentAnim();
		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s )to apply attack stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}
		FAnimMontageFullData animPack = meleeAnims->BlockRuin.Get(GetCurrentDirection());
		Multi_PlayAnim(weapon, animPack, attackStunLen);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to apply attack stun",
		           *data->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::NotifyBlocked()
{
	SetHasBlocked(true);
	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s)to apply block",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s)to apply block",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeCombinedData& currentData = meleeWeapon->GetCurrentMeleeCombinedData();

		OnMeleeBlockSound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.Block);
		Client_Blocked(CurrentDirection, currentData.Block);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to apply block",
		           *data->GetClass()->GetFName().ToString());
		return;
	}
}

/*void UAdvancedWeaponManager::ApplyBlockStun()
{
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::BlockStunned);

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(HittingTimerHandle);
	timerManager.ClearTimer(FightTimerHandle);
	timerManager.ClearTimer(EquippingTimerHandle);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s)to apply block stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s)to apply block stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeCombinedData& currentData = meleeWeapon->GetCurrentMeleeCombinedData();
		float stunLen = currentData.Block.BlockStunLen;

		const FMeleeBlockRuinAnimData& blockRuinAnimData = meleeWeapon->IsShieldEquipped()
			                                                   ? meleeAnims->Shield.BlockRuin
			                                                   : meleeAnims->BlockRuin;
		const FAnimMontageFullData& dirBlockData = blockRuinAnimData.Get(CurrentDirection);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle,
		                                       FTimerDelegate::CreateUObject(
			                                       this, &UAdvancedWeaponManager::BlockStunFinished),
		                                       stunLen, false);

		Multi_PlayAnim(meleeWeapon, dirBlockData, stunLen);
		Client_Blocked(CurrentDirection, currentData.Block);

		OnMeleeBlockSound.Broadcast(meleeWeapon, meleeAnims->SoundPack, meleeAnims->SoundPack.Block);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to apply block stun",
		           *data->GetClass()->GetFName().ToString());
		return;
	}
}*/

void UAdvancedWeaponManager::ApplyParryStun()
{
	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::ParryStunned);

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(HittingTimerHandle);
	timerManager.ClearTimer(FightTimerHandle);
	timerManager.ClearTimer(EquippingTimerHandle);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s)to apply parry stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeAnims))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s)to apply parry stun",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeCombinedData& currentData = meleeWeapon->GetCurrentMeleeCombinedData();
		float stunLen = currentData.Attack.AttackStunLen;

		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle,
		                                       FTimerDelegate::CreateUObject(
			                                       this, &UAdvancedWeaponManager::ParryStunFinished),
		                                       stunLen, false);

		OnParryStunned.Broadcast(CurrentDirection, currentData.Attack);
		Multi_CancelCurrentAnim();
		Client_ParryStun(CurrentDirection, currentData.Attack);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to apply block stun",
		           *data->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::ClearBeforeDestroy()
{
	StopWork();
	const int32 n = WeaponList.Num();
	for (int32 i = 0; i < n; ++i)
	{
		if (WeaponList[i])
		{
			WeaponList[i]->DestroyVisuals();
			WeaponList[i]->ConditionalBeginDestroy();
			WeaponList[i] = nullptr;
		}
	}
	WeaponList.Empty();
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, WeaponList, this);
}

void UAdvancedWeaponManager::StopWork()
{
	SetFightingStatus(EWeaponFightingStatus::Busy);
	SetManagingStatus(EWeaponManagingStatus::Busy);
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(EquippingTimerHandle);
	timerManager.ClearTimer(FightTimerHandle);
	timerManager.ClearTimer(HittingTimerHandle);
}

void UAdvancedWeaponManager::DropWeaponVisual(const FString& InWeaponGuid)
{
	// Should be server side call or single player 
	Multi_DropWeaponVisual(InWeaponGuid);
}

bool UAdvancedWeaponManager::RemoveWeapon(int32 InIndex)
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return false;
	}
	// TODO: Remove and destroy weapon 
	return true;
}

bool UAdvancedWeaponManager::IsBlocking() const
{
	return GetFightingStatus() ==
		EWeaponFightingStatus::BlockCharging;
}

bool UAdvancedWeaponManager::IsChargingMeleeAttack() const
{
	return GetFightingStatus() == EWeaponFightingStatus::AttackCharging;
}

EBlockResult UAdvancedWeaponManager::CanBlockIncomingDamage(UAdvancedWeaponManager* Causer)
{
	if (!IsValid(Causer))
		return EBlockResult::Invalid;

	// No weapon, no block :D
	UAbstractWeapon* wpn = GetCurrentWeapon();
	if (!IsValid(wpn))
		return EBlockResult::FullDamage;

	// Only melee weapon is able to block
	UMeleeWeapon* meleeWpn = Cast<UMeleeWeapon>(wpn);
	if (!IsValid(meleeWpn))
		return EBlockResult::FullDamage;

	// No block, no reduction :D
	if (!IsBlocking())
		return EBlockResult::FullDamage;

	const auto attackDir = Causer->GetCurrentDirection();
	const auto blockDir = GetCurrentDirection();

	auto checkBlockDir = [](EWeaponDirection attack, EWeaponDirection block)
	{
		switch (attack)
		{
		case EWeaponDirection::Forward:
		case EWeaponDirection::Backward:
			// Block direction must match for Forward and Backward attacks
			return attack == block;

		case EWeaponDirection::Right:
			// Block direction must be Left for a Right attack
			return block == EWeaponDirection::Left;

		case EWeaponDirection::Left:
			// Block direction must be Right for a Left attack
			return block == EWeaponDirection::Right;
		default:
			// If WeaponDir is invalid, return false
			return false;
		}
	};
	const bool bShield = meleeWpn->IsShieldEquipped();

	// Only without shield we should check direction
	if (!bShield)
	{
		// Check direction
		if (!checkBlockDir(attackDir, blockDir))
		{
			return EBlockResult::FullDamage;
		}
	}

	UAbstractWeapon* causerWpn = Causer->GetCurrentWeapon();

	// Target weapon must be valid
	if (!IsValid(causerWpn))
		return EBlockResult::Invalid;
	// Target weapon must be melee class
	UMeleeWeapon* meleeCauserWeapon = Cast<UMeleeWeapon>(causerWpn);
	if (!IsValid(meleeCauserWeapon))
		return EBlockResult::Invalid;

	// Causer must be in state 'Attacking'
	if (Causer->GetFightingStatus() != EWeaponFightingStatus::Attacking)
		return EBlockResult::Invalid;

	const float blockValue = EvaluateCurrentCurve();
	const float attackValue = Causer->GetCurrentHitPower();

	const uint8 blockTier = static_cast<uint8>(meleeWpn->GetData()->WeaponTier);
	const uint8 attackTier = static_cast<uint8>(meleeCauserWeapon->GetData()->WeaponTier);

	/*
	* We can only parry weapons of the same class or lower.
	* So 'High' can parry all
	* 'Medium' can only parry itself and 'Light'
	* 'Light' only itself
	 */
	const bool bIsAbleToParry = blockTier >= attackTier;

	if (bShield)
	{
		return EBlockResult::ShieldBlock;
	}

	if (attackValue >= 1.0)
	{
		return EBlockResult::Block;
	}

	if (attackValue >= blockValue)
	{
		return EBlockResult::Block;
	}

	if (attackValue < blockValue)
	{
		return bIsAbleToParry ? EBlockResult::Parry : EBlockResult::Block;
	}
	return EBlockResult::FullDamage;
}

EBlockResult UAdvancedWeaponManager::CanBlockIncomingProjectileDamage()
{
	// No weapon, no block :D
	UAbstractWeapon* wpn = GetCurrentWeapon();
	if (!IsValid(wpn))
		return EBlockResult::FullDamage;

	// Only melee weapon is able to block
	UMeleeWeapon* meleeWpn = Cast<UMeleeWeapon>(wpn);
	if (!IsValid(meleeWpn))
		return EBlockResult::FullDamage;

	// No block, no reduction :D
	if (!IsBlocking())
		return EBlockResult::FullDamage;

	if (meleeWpn->IsShieldEquipped())
	{
		return EBlockResult::ShieldProjectileBlock;
	}
	return EBlockResult::FullDamage;
}

float UAdvancedWeaponManager::BlockIncomingDamage(float InDmg, UAdvancedWeaponManager* Causer)
{
	if (!IsValid(Causer))
		return InDmg;

	UAbstractWeapon* causerWpn = Causer->GetCurrentWeapon();

	// Target weapon must be valid
	if (!IsValid(causerWpn))
		return InDmg;


	// Causer must be in state 'Attacking'
	if (Causer->GetFightingStatus() != EWeaponFightingStatus::Attacking)
		return InDmg;

	// No weapon, no block :D
	UAbstractWeapon* wpn = GetCurrentWeapon();
	if (!IsValid(wpn))
		return InDmg;

	// Only melee weapon is able to block
	UMeleeWeapon* meleeWpn = Cast<UMeleeWeapon>(wpn);
	if (!IsValid(meleeWpn))
		return InDmg;

	const FMeleeCombinedData& meleeData = meleeWpn->GetCurrentMeleeCombinedData();

	// Target weapon must be melee class
	const EWeaponTier causerTier = causerWpn->GetData()->WeaponTier;

	if (meleeData.BlockPercent.Contains(causerTier))
	{
		// reducedAmount = Amount * (1.0f - blockPercent);
		return InDmg * (1.0f - FMath::Clamp(meleeData.BlockPercent[causerTier], 0.0f, 1.0f));
	}

	return InDmg;
}

bool UAdvancedWeaponManager::CanBlockSide(const FVector& DamageSourceLocation)
{
	// No weapon, no block :D
	UAbstractWeapon* wpn = GetCurrentWeapon();
	if (!IsValid(wpn))
		return false;

	// Only melee weapon is able to block
	UMeleeWeapon* meleeWpn = Cast<UMeleeWeapon>(wpn);
	if (!IsValid(meleeWpn))
		return false;

	// Direction the actor is facing (forward vector)
	FVector actorForward = GetOwner()->GetActorForwardVector();
	actorForward.Z = 0.0f; // Ignore vertical component (Z-axis)
	actorForward.Normalize();

	// Direction to the damage source
	FVector toDamageSource = DamageSourceLocation - GetOwner()->GetActorLocation();
	toDamageSource.Z = 0.0f; // Ignore vertical component (Z-axis)
	toDamageSource.Normalize();

	// Angle between the two vectors
	const float dotProduct = FVector::DotProduct(actorForward, toDamageSource);

	//float AngleDegrees = FMath::Acos(DotProduct) * (180.0f / PI);
	const float angleDegrees = FMath::Acos(dotProduct) * (FMathd::RadToDeg);

	const float blockAngle = meleeWpn->GetCurrentMeleeCombinedData().BlockAngle;

	//TRACE(LogWeapon, "Block angle: %.1f", angleDegrees);

	return angleDegrees <= blockAngle / 2.0f;
}

UAbstractWeapon* UAdvancedWeaponManager::Weapon(int32 InIndex) const
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return nullptr;
	}
	return WeaponList[InIndex];
}

int32 UAdvancedWeaponManager::WeaponIndex(UAbstractWeapon* InWeapon)
{
	if (!IsValid(InWeapon))
		return INDEX_NONE;

	if (!WeaponList.Contains(InWeapon))
		return INDEX_NONE;

	return WeaponList.IndexOfByKey(InWeapon);
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

	const bool bIdle = ManagingStatus == EWeaponManagingStatus::Idle;
	const bool bNoWeapon = ManagingStatus == EWeaponManagingStatus::NoWeapon;
	if (!(bIdle || bNoWeapon))
	{
		return false;
	}

	// Equip is allowed if current weapon is null
	UAbstractWeapon* currentWeapon = GetCurrentWeapon();
	if (IsValid(currentWeapon))
	{
		return false;
	}

	// // Already equipped
	// UAbstractWeapon* wpn = Weapon(InIndex);
	// if (wpn == currentWeapon)
	// {
	// 	return false;
	// }

	return true;
}


bool UAdvancedWeaponManager::CanDeEquip(int32 InIndex) const
{
	if (!IsValidWeaponIndex(InIndex))
		return false;

	const bool bIdle = ManagingStatus == EWeaponManagingStatus::Idle;
	// Idle 
	if (!bIdle)
		return false;

	// Current weapon must be not null
	UAbstractWeapon* currentWeapon = GetCurrentWeapon();
	if (!IsValid(currentWeapon))
		return false;


	// Invalid current weapon
	const int32 currentIndex = GetCurrentWeaponIndex();
	if (currentIndex == INDEX_NONE)
		return false;

	// Can deEquip current only current weapon
	if (currentIndex != InIndex)
		return false;

	return true;
}

bool UAdvancedWeaponManager::CanStartAttack() const
{
	// TODO: Check if weapon can start attack (e.g. have ammo or stamina)
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	if (GetManagingStatus() != EWeaponManagingStatus::Idle)
		return false;

	if (GetFightingStatus() != EWeaponFightingStatus::Idle)
		return false;

	return true;
}

bool UAdvancedWeaponManager::CanAttack() const
{
	// TODO: Check if weapon can attack
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;
	EWeaponFightingStatus fightingStatus = GetFightingStatus();


	bool bCharging = fightingStatus == EWeaponFightingStatus::AttackCharging || fightingStatus ==
		EWeaponFightingStatus::RangeCharging;
	if (!bCharging)
		return false;

	return true;
}

bool UAdvancedWeaponManager::CanBlock() const
{
	// TODO: Check if weapon can block (melee)
	UAbstractWeapon* cur = GetCurrentWeapon();

	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	if (!cur->IsBlockAllowed())
		return false;

	const EWeaponFightingStatus status = GetFightingStatus();
	const bool bIdle = status == EWeaponFightingStatus::Idle;
	const bool bAttackCharge = status == EWeaponFightingStatus::AttackCharging || status ==
		EWeaponFightingStatus::RangeCharging;

	return bIdle || bAttackCharge;
}

bool UAdvancedWeaponManager::CanUnBlock() const
{
	// TODO: Check if weapon can unblock (melee)
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	const EWeaponFightingStatus status = GetFightingStatus();
	const bool bBlockCharge = status == EWeaponFightingStatus::BlockCharging;

	return bBlockCharge;
}

bool UAdvancedWeaponManager::CanGetShield() const
{
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(cur);

	if (!IsValid(meleeWeapon))
		return false;

	// Weapon must have shield
	if (!meleeWeapon->IsWeaponShieldSupported())
		return false;

	// Shield must be not equipped
	if (meleeWeapon->IsShieldEquipped())
		return false;

	const EWeaponManagingStatus status = GetManagingStatus();
	const bool bIdle = status == EWeaponManagingStatus::Idle;

	return bIdle;
}

bool UAdvancedWeaponManager::CanRemoveShield() const
{
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(cur);

	if (!IsValid(meleeWeapon))
		return false;

	// Weapon must have shield
	if (!meleeWeapon->IsWeaponShieldSupported())
		return false;

	// Shield must be equipped
	if (!meleeWeapon->IsShieldEquipped())
		return false;

	const EWeaponManagingStatus status = GetManagingStatus();
	const bool bIdle = status == EWeaponManagingStatus::Idle;

	return bIdle;
}

void UAdvancedWeaponManager::ProcessWeaponDamage(AActor* Causer, float Amount,
                                                 const FHitResult& HitResult,
                                                 TSubclassOf<UDamageType> DamageType,
                                                 EDamageReturn& OutDamageReturn, float& OutDamage)
{
	OutDamageReturn = EDamageReturn::Failed;
	OutDamage = 0.0f;

	UAdvancedWeaponManager* causerWpnManager = Causer->FindComponentByClass<UAdvancedWeaponManager>();
	EBlockResult blockResult = this->CanBlockIncomingDamage(causerWpnManager);

	if (blockResult == EBlockResult::Invalid)
	{
		TRACEERROR(LogWeapon, "Failed to block incoming damage (%s)", *UEnum::GetValueAsString(blockResult))
		return;
	}

	// So we are ready to parry or block something
	if (blockResult != EBlockResult::FullDamage)
	{
		// Wide angle of attack
		if (!this->CanBlockSide(HitResult.TraceStart))
		{
			blockResult = EBlockResult::FullDamage;
		}
	}

	float realDmg = Amount;
	if (blockResult == EBlockResult::Invalid)
	{
		return;
	}
	if (blockResult == EBlockResult::Parry)
	{
		causerWpnManager->ApplyParryStun();
		this->StartParry(CurrentDirection);
		OutDamageReturn = EDamageReturn::Alive;
		OutDamage = 0.01 * realDmg;
		return;
	}

	if (blockResult != EBlockResult::FullDamage)
	{
		realDmg = this->BlockIncomingDamage(Amount, causerWpnManager);
	}

	IWeaponManagerOwner::Execute_ApplyDamage(GetOwner(), Causer, realDmg, HitResult,
	                                         DamageType,
	                                         OutDamageReturn, OutDamage);

	if (blockResult == EBlockResult::Block)
	{
		// Sound, effect, etc
		this->NotifyBlocked();
	}

	//EDamageReturn dmgReturnResult = EDamageReturn::Failed;
	// Apply damage
	/*if (blockResult == EBlockResult::FullDamage ||
		blockResult == EBlockResult::PartialDamage ||
		blockResult == EBlockResult::PartialShieldDamage ||
		blockResult == EBlockResult::FullDamageBlockRuin)
	{
		// Block some of the damage
		if (blockResult == EBlockResult::PartialDamage ||
			blockResult == EBlockResult::PartialShieldDamage ||
			blockResult == EBlockResult::FullDamageBlockRuin)
		{
			realDmg = this->BlockIncomingDamage(Amount, causerWpnManager);
			/*TRACE(LogWeapon, "%s Blocked some damage from %s. Incoming: %.1f. After block: %.1f",
			      *this->GetFName().ToString(),
			      *Causer->GetFName().ToString(),
			      Amount,
			      realDmg);#1#
		}

		if (GetOwner()->Implements<UWeaponManagerOwner>())
		{
			IWeaponManagerOwner::Execute_ApplyDamage(GetOwner(), Causer, realDmg, HitResult,
			                                         DamageType,
			                                         OutDamageReturn, OutDamage);
		}
		// nothing, just keep blocking by shield
		if (blockResult == EBlockResult::PartialShieldDamage)
		{
			//TODO: Remove shield if zero stamina
		}
		// Remove block
		else if (blockResult == EBlockResult::FullDamageBlockRuin)
		{
			this->ApplyBlockStun();
		}
		// Both get stunned
		else if (blockResult == EBlockResult::PartialDamage)
		{
			this->ApplyBlockStun();
			causerWpnManager->NotifyEnemyMeleeBlocked();
		}
		return;
	}
	else if (blockResult == EBlockResult::FullShieldBlock)
	{
		causerWpnManager->NotifyEnemyMeleeBlocked();
		//TODO: Event to remove stamina and maybe destroy shield
		OutDamageReturn = EDamageReturn::Alive;
		OutDamage = 0.0f;
		return;
	}
	else if (blockResult == EBlockResult::Parry)
	{
		causerWpnManager->ApplyParryStun();
		this->StartParry(CurrentDirection);
		OutDamageReturn = EDamageReturn::Alive;
		OutDamage = 0.0f;
		return;
	}*/
}

void UAdvancedWeaponManager::ProcessProjectileDamage(AActor* Causer, float Amount, const FHitResult& HitResult,
                                                     TSubclassOf<UDamageType> DamageType,
                                                     EDamageReturn& OutDamageReturn, float& OutDamage)
{
	OutDamageReturn = EDamageReturn::Failed;
	OutDamage = 0.0f;
	EBlockResult blockResult = EBlockResult::FullDamage;
	if (!this->CanBlockSide(HitResult.TraceStart))
	{
		blockResult = EBlockResult::FullDamage;
	}
	else
	{
		blockResult = CanBlockIncomingProjectileDamage();
	}

	if (blockResult == EBlockResult::ShieldProjectileBlock)
	{
		if (UMeleeWeapon* melee = Cast<UMeleeWeapon>(GetCurrentWeapon()))
		{
			if (UMeleeWeaponDataAsset* meleeData = melee->GetMeleeData())
			{
				Amount = Amount * (1.0f - FMath::Clamp(meleeData->ShieldProjectileBlockPercent, 0.0f, 1.0f));
			}
		}
	}

	if (GetOwner()->Implements<UWeaponManagerOwner>())
	{
		IWeaponManagerOwner::Execute_ApplyDamage(GetOwner(), Causer, Amount, HitResult,
		                                         DamageType,
		                                         OutDamageReturn, OutDamage);
	}

	/*if (blockResult == EBlockResult::FullShieldBlock)
	{
		OutDamageReturn = EDamageReturn::Alive;
		OutDamage = 0.0f;
		return;
		//TODO: Event to remove stamina and maybe destroy shield
	}
	// Full damage
	else
	{
		//TODO: Extract method
		if (blockResult == EBlockResult::PartialShieldDamage)
		{
			if (UMeleeWeapon* melee = Cast<UMeleeWeapon>(GetCurrentWeapon()))
			{
				if (UMeleeWeaponDataAsset* meleeData = melee->GetMeleeData())
				{
					Amount = Amount * (1.0f - FMath::Clamp(meleeData->ShieldProjectileBlockPercent, 0.0f, 1.0f));
				}
			}
		}
		OutDamageReturn = EDamageReturn::Failed;
		OutDamage = 0.0f;
		if (GetOwner()->Implements<UWeaponManagerOwner>())
		{
			IWeaponManagerOwner::Execute_ApplyDamage(GetOwner(), Causer, Amount, HitResult,
			                                         DamageType,
			                                         OutDamageReturn, OutDamage);
		}
		return;
	}*/
}


bool UAdvancedWeaponManager::CanChange(int32 InIndex)
{
	if (!IsValid(GetCurrentWeapon()))
	{
		// Can not change weapon if we don't have weapon
		// You should use Equip method
		// @see Equip
		return false;
	}
	const int32 curIndex = GetCurrentWeaponIndex();
	if (curIndex == InIndex)
	{
		// Can not change weapon to current
		return false;
	}
	const bool bDeEquip = CanDeEquip(curIndex);
	return bDeEquip;
}

void UAdvancedWeaponManager::TryEquipProxy(int32 InIndex)
{
	if (CanChange(InIndex))
	{
		Server_Change(InIndex);
	}
	else if (CanEquip(InIndex))
	{
		Server_Equip(InIndex);
	}
}

void UAdvancedWeaponManager::TryDeEquipProxy(int32 InIndex)
{
	if (!CanDeEquip(InIndex))
		return;

	Server_DeEquip(InIndex);
}

void UAdvancedWeaponManager::TryRemoveShieldProxy()
{
	if (!CanRemoveShield())
		return;

	Server_RemoveShield();
}

void UAdvancedWeaponManager::TryGetShieldProxy()
{
	if (!CanGetShield())
		return;

	Server_GetShield();
}

void UAdvancedWeaponManager::TrySwapShieldProxy()
{
	if (CanGetShield())
	{
		Server_GetShield();
	}
	else if (CanRemoveShield())
	{
		Server_RemoveShield();
	}
}

void UAdvancedWeaponManager::RequestDirectedAttackProxy(EWeaponDirection InDirection)
{
	if (!CanStartAttack())
		return;
	Server_StartAttack(InDirection);
}

void UAdvancedWeaponManager::RequestSimpleAttackProxy()
{
	if (!CanStartAttack())
		return;
	Server_StartAttackSimple();
}

void UAdvancedWeaponManager::RequestAttackReleasedProxy()
{
	if (!CanAttack())
		return;
	Server_Attack();
}

void UAdvancedWeaponManager::RequestBlockProxy(EWeaponDirection InDirection)
{
	if (!CanBlock())
		return;
	Server_Block(InDirection);
}

void UAdvancedWeaponManager::RequestBlockReleasedProxy()
{
	if (!CanUnBlock())
		return;
	Server_UnBlock();
}
