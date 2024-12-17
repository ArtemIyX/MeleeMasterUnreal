// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/RangeWeaponDataAsset.h"

#include "Actors/BasicProjectile.h"

URangeWeaponDataAsset::URangeWeaponDataAsset() : Super(), BasicDamage(25.0)
{
	AssetType = FName(TEXT("RangeWeaponData"));
	//ProjectileClass = ABasicProjectile::StaticClass();
}
