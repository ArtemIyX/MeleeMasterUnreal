// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/MeleeWeaponDataAsset.h"
#include "Components/AdvancedWeaponManager.h"

const FWeaponCurveData& FMeleeWeaponCurveData::Get(EWeaponDirection InDir)
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
	return Forward;
}

UMeleeWeaponDataAsset::UMeleeWeaponDataAsset() : Super(), Attack(), Block()
{
	AssetType = FName(TEXT("MeleeWeaponData"));
}
