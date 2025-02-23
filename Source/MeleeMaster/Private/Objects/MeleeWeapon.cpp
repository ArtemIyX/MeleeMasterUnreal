// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/MeleeWeapon.h"

#include "Data/MeleeWeaponDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

UMeleeWeapon::UMeleeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
                                                                          bShieldEquipped(true)
{
}

void UMeleeWeapon::OnRep_ShieldEquipped()
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
}
