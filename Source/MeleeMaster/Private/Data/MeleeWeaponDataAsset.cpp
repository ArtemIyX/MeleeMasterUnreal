// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/MeleeWeaponDataAsset.h"
#include "Components/AdvancedWeaponManager.h"
#include "Data/WeaponHitPathAsset.h"

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

const FMeleeAttackCurveData& FMeleeAttackData::Get(EWeaponDirection InDir)
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

const FMeleeBlockCurveData& FMeleeBlockData::Get(EWeaponDirection InDir)
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

UMeleeWeaponDataAsset::UMeleeWeaponDataAsset() : Super(), Attack(), Block()
{
	AssetType = FName(TEXT("MeleeWeaponData"));
}
