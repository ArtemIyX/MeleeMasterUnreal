// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/AdvancedWeaponManager.h"

#include "MeleeMaster.h"
#include "Actors/WeaponVisual.h"
#include "Data/MeleeWeaponAnimDataAsset.h"
#include "Data/MeleeWeaponDataAsset.h"
#include "Data/WeaponAnimationDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Data/WeaponHitPathAsset.h"
#include "Data/Interfaces/DamageableEntity.h"
#include "Data/Interfaces/DamageManager.h"
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

void UAdvancedWeaponManager::SetSavedGuid(FString Value)
{
	this->SavedGuid = Value;
}

void UAdvancedWeaponManager::SetChargingCurve(UCurveFloat* InCurve)
{
	this->CurrentCurve = InCurve;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, CurrentCurve, this);
}

void UAdvancedWeaponManager::SetChargeFinished(float InFinishTime)
{
	this->ChargeWillBeFinished = InFinishTime;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ChargeWillBeFinished, this);
}

void UAdvancedWeaponManager::SetChargeStarted(float InStartTime)
{
	this->ChargeStarted = InStartTime;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAdvancedWeaponManager, ChargeStarted, this);
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

void UAdvancedWeaponManager::OnRep_CurrentCurve()
{
}

void UAdvancedWeaponManager::OnRep_Charge()
{
}

void UAdvancedWeaponManager::OnRep_ChargeStarted()
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
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentDirection, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, CurrentCurve, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, ChargeWillBeFinished, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAdvancedWeaponManager, ChargeStarted, Params);
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

float UAdvancedWeaponManager::EvaluateCurrentCurve() const
{
	EWeaponFightingStatus fightStatus = GetFightingStatus();
	if (fightStatus == EWeaponFightingStatus::AttackCharging || fightStatus == EWeaponFightingStatus::BlockCharging)
	{
		if (!IsValid(CurrentCurve))
			return MinimalCurveValue;
		// Current time
		const float currentServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
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
		return MinimalCurveValue;
	}
	return 0.0f;
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
		if (IsValid(visualActor))
		{
			visualActor->SetGuidString(weaponGuid);
			actors.Add(visualActor);
		}
	}
	InAbstractWeapon->SetVisual(actors);

	// Attach to owner (on server)
	for (int32 i = 0; i < actors.Num(); ++i)
	{
		actors[i]->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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
				}
			}
		}
	}


	UWeaponDataAsset* data = InWeapon->GetData();

	TArray<FMeleeHitDebugData> debugArr;
	for (TTuple<AActor*, FHitResult> el : hitMap)
	{
		if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(InWeapon))
		{
			UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
			if (!IsValid(meleeWeaponData))
			{
				TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to process hit",
				           *data->GetClass()->GetFName().ToString());
				continue;
			}
			const FMeleeAttackCurveData& attackData = meleeWeaponData->Attack.Get(CurrentDirection);

			float dmg = attackData.BasicDamage * HitPower;
			if (bDebugMeleeHits)
			{
				debugArr.Add(FMeleeHitDebugData(el.Value.Location, attackData.BasicDamage, HitPower));
			}
			TSubclassOf<UDamageType> dmgType = attackData.DamageType;
			IDamageManager::Execute_RequestDamage(gm, GetOwner(), el.Key, dmg, el.Value, dmgType);
		}
		else
		{
			TRACEERROR(LogWeapon, "Invalid weapon class (%s) to process hit",
			           *InWeapon->GetClass()->GetFName().ToString());
			continue;
		}
	}
	if (bDebugMeleeHits && debugArr.Num() > 0)
	{
		Multi_DebugHit(debugArr);
	}
}


void UAdvancedWeaponManager::Multi_DebugHit_Implementation(const TArray<FMeleeHitDebugData>& InData)
{
	if (GetOwner()->HasAuthority())
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

		const FMeleeAttackCurveData& attack = meleeWeaponData->Attack.Get(CurrentDirection);
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

void UAdvancedWeaponManager::HitFinished()
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
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to start post attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attack = meleeWeaponData->Attack.Get(CurrentDirection);
		float postAttackTime = attack.PostAttackLen;
		TDelegate<TDelegate<void(), FNotThreadSafeNotCheckedDelegateUserPolicy>::RetValType(),
		          FNotThreadSafeNotCheckedDelegateUserPolicy> delegate = FTimerDelegate::CreateUObject(
			this, &UAdvancedWeaponManager::PostAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, postAttackTime, false);
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

		const FMeleeAttackCurveData& attackData = meleeWeaponData->Attack.Get(CurrentDirection);
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
		FRotator controlRot = pawnOwner->GetControlRotation();
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
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to melee attack",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to melee attack",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}
		const FMeleeAttackCurveData& attackData = meleeWeaponData->Attack.Get(CurrentDirection);

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

		// Looped line-trace method
		HitNum = 0;

		const float frequency = attackData.HittingTime / FMath::Clamp(hitPath->Data.Elements.Num(), 1,
		                                                              TNumericLimits<int32>::Max() - 1);
		auto hittingDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::MeleeHitProcedure);
		GetWorld()->GetTimerManager().SetTimer(HittingTimerHandle, hittingDelegate, frequency, true);

		const FMeleeAttackAnimMontageData& attackAnim = meleeAnims->Attack.Get(CurrentDirection);
		Multi_PlayAnim(meleeWeapon, attackAnim, attackData.HittingTime, true,
		               attackAnim.AttackSection);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to attack",
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

	SetManagingStatus(EWeaponManagingStatus::Busy);
	SetFightingStatus(EWeaponFightingStatus::BlockCharging);
	SetDirection(InDirection);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;
	if (UMeleeWeapon* meleeWeapon = Cast<UMeleeWeapon>(weapon))
	{
		UMeleeWeaponDataAsset* meleeWeaponData = Cast<UMeleeWeaponDataAsset>(data);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon data class (%s) to block",
			           *data->GetClass()->GetFName().ToString());
			return;
		}

		UMeleeWeaponAnimDataAsset* meleeAnims = Cast<UMeleeWeaponAnimDataAsset>(anims);
		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to block",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeBlockCurveData& blockData = meleeWeaponData->Block.Get(CurrentDirection);
		float currentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		SetChargeStarted(currentTime);
		SetChargeFinished(currentTime + blockData.CurveTime);
		UCurveFloat* curve = blockData.Curve.LoadSynchronous();
		SetChargingCurve(curve);
		OnStartedChargingBlock.Broadcast(meleeWeapon, GetChargingCurve(), GetChargingFinishTime());

		const FMeleeBlockAnimMontageData& blockAnimData = meleeAnims->Block.Get(CurrentDirection);

		Multi_PlayAnim(meleeWeapon, blockAnimData, blockAnimData.LiftingTime);
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
		const FMeleeBlockCurveData& blockData = meleeWeaponData->Block.Get(CurrentDirection);
		auto delegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::PostBlockFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, delegate, blockData.PostBlockLen, false);
		Multi_CancelCurrentAnim();
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to unblock",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Server_DeEquip_Implementation(int32 InIndex)
{
	if (!CanDeEquip(InIndex))
		return;

	// Mark status
	SetManagingStatus(EWeaponManagingStatus::DeEquipping);

	UAbstractWeapon* weapon = GetCurrentWeapon();
	UWeaponDataAsset* data = weapon->GetData();
	UWeaponAnimationDataAsset* anims = data->Animations;

	auto delegate = FTimerDelegate::CreateLambda([this]()
	{
		SetManagingStatus(EWeaponManagingStatus::NoWeapon);
		SetFightingStatus(EWeaponFightingStatus::Idle);
		UAbstractWeapon* curWeapon = GetCurrentWeapon();
		FString guid = curWeapon->GetGUIDString();
		SetSavedGuid(guid);
		Multi_AttachBack(guid);
		SetCurrentWeaponPtr(nullptr);
	});

	GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, data->DeEquipTime, false);

	if (IsValid(anims))
	{
		Multi_PlayAnim(weapon, anims->DeEquip, data->DeEquipTime);
	}
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
		Multi_AttachBack(SavedGuid);
	});
	GetWorld()->GetTimerManager().SetTimer(EquippingTimerHandle, delegate, data->EquipTime, false);

	if (IsValid(anims))
	{
		Multi_PlayAnim(weapon, anims->Equip, data->EquipTime);
	}
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

		if (!IsValid(meleeWeaponData))
		{
			TRACEERROR(LogWeapon, "Invalid weapon anim data class (%s) to start melee attack",
			           *anims->GetClass()->GetFName().ToString());
			return;
		}

		const FMeleeAttackCurveData& attackData = meleeWeaponData->Attack.Get(InDirection);

		auto initialDelegate = FTimerDelegate::CreateUObject(this, &UAdvancedWeaponManager::PreAttackFinished);
		GetWorld()->GetTimerManager().SetTimer(FightTimerHandle, initialDelegate, attackData.PreAttackLen, false);

		FMeleeAttackAnimMontageData attackAnim = meleeAnims->Attack.Get(InDirection);
		FAnimMontageFullData montageData = attackAnim;
		Multi_PlayAnim(weapon, montageData, attackData.PreAttackLen);
	}
	else
	{
		TRACEERROR(LogWeapon, "Invalid weapon class (%s) to start attack",
		           *weapon->GetClass()->GetFName().ToString());
		return;
	}
}

void UAdvancedWeaponManager::Multi_PlayAnim_Implementation(
	UAbstractWeapon* InWeapon,
	const FAnimMontageFullData& AnimSet,
	float Time,
	bool bUseSection,
	const FName& Section)
{
	SetSavedGuid(InWeapon->GetGUIDString());
	FAnimPlayData data;
	if (bUseSection)
	{
		data = FAnimPlayData(InWeapon, AnimSet, Time, Section);
	}
	else
	{
		data = FAnimPlayData(InWeapon, AnimSet, Time);
	}
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		OnFpAnim.Broadcast(data);
	}
	else
	{
		OnTpAnim.Broadcast(data);
	}
}

void UAdvancedWeaponManager::Multi_AttachHand_Implementation()
{
	// Skip servers, it is already attached to actor
	if (GetOwner()->HasAuthority())
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
	if (GetOwner()->HasAuthority())
		return;

	UAbstractWeapon* wpn = WeaponByGuid(SavedGuid);
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
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		OnCancelCurrentFpAnim.Broadcast();
	}
	else
	{
		OnCancelCurrentTpAnim.Broadcast();
	}
}


void UAdvancedWeaponManager::AttachBack(AWeaponVisual* InVisual)
{
	// Skip servers, it is already attached to actor
	if (GetOwner()->HasAuthority())
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

	ENetRole role = owner->GetLocalRole();
	bool bIsLocallyControlled = (role == ROLE_AutonomousProxy);

	FName backSocket = InVisual->GetBackSocket();
	InVisual->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                            backSocket);

	if (bIsLocallyControlled)
	{
		if (USkeletalMeshComponent* visualSkeletalMesh = InVisual->GetSkeletalMeshComponent())
		{
			visualSkeletalMesh->CastShadow = false;
			visualSkeletalMesh->bCastHiddenShadow = false;
			visualSkeletalMesh->SetVisibility(false);
			visualSkeletalMesh->SetHiddenInGame(true);
			InVisual->SetHidden(true);
		}
	}
}

void UAdvancedWeaponManager::AttachHand(AWeaponVisual* InVisual)
{
	// Skip servers, it is already attached to actor
	if (GetOwner()->HasAuthority())
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

	const ENetRole role = owner->GetLocalRole();
	const bool bIsLocallyControlled = (role == ROLE_AutonomousProxy);

	const FName handSocket = InVisual->GetHandSocket();
	InVisual->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                            handSocket);

	if (bIsLocallyControlled)
	{
		if (USkeletalMeshComponent* visualSkeletalMesh = InVisual->GetSkeletalMeshComponent())
		{
			visualSkeletalMesh->CastShadow = false;
			visualSkeletalMesh->bCastHiddenShadow = false;
			visualSkeletalMesh->SetVisibility(true);
			visualSkeletalMesh->SetHiddenInGame(false);
			InVisual->SetHidden(false);
		}
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

bool UAdvancedWeaponManager::RemoveWeapon(int32 InIndex)
{
	if (!WeaponList.IsValidIndex(InIndex))
	{
		return false;
	}
	// TODO: Remove
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

	const bool bIdle = ManagingStatus == EWeaponManagingStatus::Idle;
	const bool bNoWeapon = ManagingStatus == EWeaponManagingStatus::NoWeapon;
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
	// TODO: Check if weapon can start attack
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
	if (GetFightingStatus() != EWeaponFightingStatus::AttackCharging)
		return false;

	return true;
}

bool UAdvancedWeaponManager::CanBlock()
{
	// TODO: Check if weapon can block (melee)
	UAbstractWeapon* cur = GetCurrentWeapon();
	if (!IsValid(cur))
		return false;
	if (!cur->IsValidData())
		return false;

	const EWeaponFightingStatus status = GetFightingStatus();
	const bool bIdle = status == EWeaponFightingStatus::Idle;
	const bool bAttackCharge = status == EWeaponFightingStatus::AttackCharging;

	return bIdle || bAttackCharge;
}

bool UAdvancedWeaponManager::CanUnBlock()
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

void UAdvancedWeaponManager::TryEquipProxy(int32 InIndex)
{
	if (!CanEquip(InIndex))
		return;

	Server_Equip(InIndex);
}

void UAdvancedWeaponManager::TryDeEquipProxy(int32 InIndex)
{
	if (!CanDeEquip(InIndex))
		return;

	Server_DeEquip(InIndex);
}

void UAdvancedWeaponManager::RequestAttackProxy(EWeaponDirection InDirection)
{
	if (!CanStartAttack())
		return;
	Server_StartAttack(InDirection);
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
