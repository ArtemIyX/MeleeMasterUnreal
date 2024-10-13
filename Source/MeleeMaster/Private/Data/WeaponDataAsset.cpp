// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/WeaponDataAsset.h"

#include "Actors/WeaponVisual.h"
#include "Objects/AbstractWeapon.h"

UWeaponDataAsset::UWeaponDataAsset()
{
	AssetType = "WeaponData";
}

bool UWeaponDataAsset::IsValidToCreate() const
{
	// Valid classes
	for (TTuple<FName, TSubclassOf<AWeaponVisual>> el : Visuals)
	{
		bool bVisual = (bool)el.Value;
		if (!bVisual)
			return false;
	}
	// Valid weapon class
	bool bWeaponClass = (bool)WeaponClass;
	return bWeaponClass;
}
