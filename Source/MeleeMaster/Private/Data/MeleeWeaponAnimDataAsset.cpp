// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/MeleeWeaponAnimDataAsset.h"
#include "Components/AdvancedWeaponManager.h"

const FMeleeAttackAnimMontageData& FMeleeAttackAnimData::Get(EWeaponDirection InDir) const
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

const FMeleeBlockAnimMontageData& FMeleeBlockAnimData::Get(EWeaponDirection InDir) const
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

UMeleeWeaponAnimDataAsset::UMeleeWeaponAnimDataAsset(): Shield()
{
	AssetType = TEXT("MeleeWeaponAnim");
}
