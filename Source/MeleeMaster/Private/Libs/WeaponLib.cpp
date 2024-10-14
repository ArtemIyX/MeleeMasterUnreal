// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Libs/WeaponLib.h"

float UWeaponLib::CalculatePlayRate(float InitialSeconds, float ResultSeconds)
{
	return FMath::Clamp(InitialSeconds / ResultSeconds, 0.0f, TNumericLimits<float>::Max());
}
