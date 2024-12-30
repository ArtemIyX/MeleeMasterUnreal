// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/WeaponModifierManager.h"

#include "Components/AdvancedWeaponManager.h"


void AWeaponModifierManager::AttackCharging_Implementation(UAbstractWeapon* InWeapon, float InPower)
{
}

void AWeaponModifierManager::BlockCharging_Implementation(UAbstractWeapon* InWeapon, float InPower)
{
}

void AWeaponModifierManager::Clear_Implementation()
{
}

void AWeaponModifierManager::IdleState_Implementation(UAbstractWeapon* CurrentWeapon)
{
}

void AWeaponModifierManager::SetWeaponManager(UAdvancedWeaponManager* InWeaponManager)
{
	WeaponManagerPtr = InWeaponManager;
}

void AWeaponModifierManager::MeleeAttack_Implementation(UAbstractWeapon* InWeapon)
{
	
}

void AWeaponModifierManager::RangeAttack_Implementation(UAbstractWeapon* InWeapon)
{
	
}

void AWeaponModifierManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Clear();
	Super::EndPlay(EndPlayReason);
}
