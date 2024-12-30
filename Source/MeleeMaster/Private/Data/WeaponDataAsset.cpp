// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/WeaponDataAsset.h"

#include "Actors/WeaponVisual.h"
#include "Objects/AbstractWeapon.h"
#include "Objects/WeaponModifierManager.h"

UWeaponDataAsset::UWeaponDataAsset(): Animations(nullptr), EquipTime(2), DeEquipTime(2), WeaponTier(EWeaponTier::Medium)
{
	AssetType = "WeaponData";

	bAttackDirected = true;
	bBlockAllowed = true;
	bBlockDirected = true;
	VisualModifier = AWeaponModifierManager::StaticClass();
}

bool UWeaponDataAsset::IsValidToCreate() const
{
	// Valid classes
	for (TSubclassOf<AWeaponVisual> el : Visuals)
	{
		bool bVisual = (bool)el;
		if (!bVisual)
			return false;
	}
	// Valid weapon class
	bool bWeaponClass = (bool)WeaponClass;
	return bWeaponClass;
}
