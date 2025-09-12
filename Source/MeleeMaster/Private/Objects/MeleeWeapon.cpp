// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/MeleeWeapon.h"

#include "Components/AdvancedWeaponManager.h"
#include "Data/MeleeWeaponDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Data/WeaponHitPathAsset.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

UMeleeWeapon::UMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                          bShieldEquipped(true), bShieldHasDropped(0)
{
}

void UMeleeWeapon::OnRep_ShieldEquipped()
{
}

void UMeleeWeapon::OnRep_ShieldDurability()
{
}

void UMeleeWeapon::OnRep_ShieldDropped()
{
}

void UMeleeWeapon::SetShieldEquipped(bool bInValue)
{
	if (IsWeaponShieldSupported())
	{
		bShieldEquipped = bInValue;
		MARK_PROPERTY_DIRTY_FROM_NAME(UMeleeWeapon, bShieldEquipped, this);
	}
}

bool UMeleeWeapon::IsWeaponShieldSupported() const
{
	if (IsValidData())
	{
		if (UMeleeWeaponDataAsset* meleeData = GetMeleeData())
		{
			return meleeData->bHasShield;
		}
	}
	return false;
}

void UMeleeWeapon::SetShieldDurability(float InValue)
{
	ShieldDurability = FMath::Clamp(InValue, 0.0f, 1.0f);
	MARK_PROPERTY_DIRTY_FROM_NAME(UMeleeWeapon, ShieldDurability, this);

	if (ShieldDurability <= 0.0f)
	{
		if (WeaponManagerOwner.IsValid())
		{
			WeaponManagerOwner->NotifyShieldDurabilityLost();
		}
		SetShieldDropFlag(true);
	}
	else if (ShieldDurability >= 1.0f)
	{
		SetShieldDropFlag(false);
	}
}

void UMeleeWeapon::SetShieldDropFlag(bool InFlag)
{
	bShieldHasDropped = InFlag;
	MARK_PROPERTY_DIRTY_FROM_NAME(UMeleeWeapon, bShieldHasDropped, this);
}

void UMeleeWeapon::IncreaseShieldDurability()
{
	SetShieldDurability(GetShieldDurability() + ShieldIncreaseValue);
}

void UMeleeWeapon::DecreaseShieldDurability()
{
	SetShieldDurability(GetShieldDurability() - ShieldDecreaseValue);
}

void UMeleeWeapon::ObjectEndPlay()
{
	Super::ObjectEndPlay();
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(ShieldDurabilityTimerHandle);
	}
}

float UMeleeWeapon::ConvertIncomingDamageToShieldDamage_Implementation(float InDamage)
{
	return FMath::Abs(InDamage) / 200.0;
}

void UMeleeWeapon::ProcessShieldDamage_Implementation(float InDamage)
{
	SetShieldDurability(GetShieldDurability() - ConvertIncomingDamageToShieldDamage(FMath::Abs(InDamage)));
}

void UMeleeWeapon::StartDecreasingShield(FTimerManager& InTimerManager)
{
	InTimerManager.ClearTimer(ShieldDurabilityTimerHandle);
	InTimerManager.SetTimer(ShieldDurabilityTimerHandle, FTimerDelegate::CreateUObject(this,
		                        &UMeleeWeapon::DecreaseShieldDurability), ShieldDecreaseRate, true);
}

void UMeleeWeapon::StartIncreasingShield(FTimerManager& InTimerManager)
{
	InTimerManager.ClearTimer(ShieldDurabilityTimerHandle);
	InTimerManager.SetTimer(ShieldDurabilityTimerHandle, FTimerDelegate::CreateUObject(this,
		                        &UMeleeWeapon::IncreaseShieldDurability), ShieldIncreaseRate, true);
}


bool UMeleeWeapon::IsShieldEquipped() const
{
	if (IsValidData())
	{
		if (UMeleeWeaponDataAsset* meleeData = GetMeleeData())
		{
			return meleeData->bHasShield && bShieldEquipped;
		}
	}
	return false;
}

float UMeleeWeapon::GetTotalDamagePerDirection(EWeaponDirection InDir) const
{
	const FMeleeCombinedData& attackData = GetCurrentMeleeCombinedData();
	const FMeleeAttackCurveData& dirData = attackData.Attack.Get(InDir);
	if (dirData.bDamageForFullPath)
	{
		return dirData.BasicDamage;
	}

	const float basicDmg = dirData.BasicDamage;
	if (UWeaponHitPathAsset* hitPath = dirData.HitPath)
	{
		int32 num = hitPath->Data.Elements.Num();
		return basicDmg * num;
	}

	return basicDmg;
}

UMeleeWeaponDataAsset* UMeleeWeapon::GetMeleeData() const
{
	return Cast<UMeleeWeaponDataAsset>(GetData());
}

const FMeleeCombinedData& UMeleeWeapon::GetCurrentMeleeCombinedData() const
{
	UMeleeWeaponDataAsset* meleeData = GetMeleeData();
	checkf(IsValid(meleeData), TEXT("Melee data must be valid"));

	if (IsShieldEquipped())
	{
		return meleeData->Shield;
	}
	else
	{
		return meleeData->Base;
	}
}

void UMeleeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams params;
	params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UMeleeWeapon, bShieldEquipped, params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UMeleeWeapon, ShieldDurability, params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UMeleeWeapon, bShieldHasDropped, params);
}
