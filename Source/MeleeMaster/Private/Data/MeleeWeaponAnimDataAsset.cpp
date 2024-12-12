// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/MeleeWeaponAnimDataAsset.h"

#include "Actors/ArrowVisual.h"
#include "Components/AdvancedWeaponManager.h"

const FAttackAnimMontageData& FMeleeAttackAnimData::Get(EWeaponDirection InDir) const
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

const FAnimMontageFullData& FMeleeBlockRuinAnimData::Get(EWeaponDirection InDir) const
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
}

FBowArrowData::FBowArrowData()
{
	VisualActorClass = AArrowVisual::StaticClass();
	HandSocket = TEXT("SOCK_");
}

UMeleeWeaponAnimDataAsset::UMeleeWeaponAnimDataAsset(): Shield()
{
	AssetType = TEXT("MeleeWeaponAnim");
}
