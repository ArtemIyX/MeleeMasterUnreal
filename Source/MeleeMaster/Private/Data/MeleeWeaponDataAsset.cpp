// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/MeleeWeaponDataAsset.h"
#include "Components/AdvancedWeaponManager.h"
#include "Data/WeaponHitPathAsset.h"

TSubclassOf<UCameraShakeBase> FDirectionCameraShakes::Get(EWeaponDirection InWeaponDirection) const
{
	switch (InWeaponDirection)
	{
	case EWeaponDirection::Forward: return Forward;
	case EWeaponDirection::Backward: return Backward;
	case EWeaponDirection::Right: return Right;
	case EWeaponDirection::Left: return Left;
	default: ;
	}
	return Forward;
}

float FMeleeAttackCurveData::GetDamage() const
{
	if (bDamageForFullPath && HitPath)
	{
		if (HitPath->Data.Elements.Num() > 0)
		{
			return BasicDamage / HitPath->Data.Elements.Num();
		}
	}
	return BasicDamage;
}

const FMeleeAttackCurveData& FMeleeAttackDataBase::Get(EWeaponDirection InDir) const
{
	switch (InDir)
	{
	case EWeaponDirection::Forward:
		return Forward;
	case EWeaponDirection::Backward:
		return Backward;
	case EWeaponDirection::Right:
		return Right;
	case EWeaponDirection::Left:
		return Left;
	default: return Forward;
	}
	//return Forward;
}

FMeleeBlockData::FMeleeBlockData()
{
}

const FMeleeBlockCurveData& FMeleeBlockData::Get(EWeaponDirection InDir) const
{
	switch (InDir)
	{
	case EWeaponDirection::Forward:
		return Forward;
	case EWeaponDirection::Backward:
		return Backward;
	case EWeaponDirection::Right:
		return Right;
	case EWeaponDirection::Left:
		return Left;
	default: return Forward;
	}
	//return Forward;
}

FMeleeCombinedData::FMeleeCombinedData()
{
	BlockPercent = {
		{EWeaponTier::High, 0.2f},
		{EWeaponTier::Medium, 0.25f},
		{EWeaponTier::Light, 0.35f},
	};
}

UMeleeWeaponDataAsset::UMeleeWeaponDataAsset() : Super()
{
	AssetType = FName(TEXT("MeleeWeaponData"));
	bHasShield = false;
}
