// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/LongRangeWeapon.h"

#include "Data/RangeWeaponDataAsset.h"
#include "Data/WeaponDataAsset.h"

URangeWeaponDataAsset* ULongRangeWeapon::GetRangeData() const
{
	return Cast<URangeWeaponDataAsset>(this->GetData());
}
