﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/AnimNotifies/AttachCurrentWeaponToHand.h"

#include "MeleeMaster.h"
#include "Components/AdvancedWeaponManager.h"
#include "Objects/AbstractWeapon.h"
#include "Subsystems/LoggerLib.h"

void UAttachCurrentWeaponToHand::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* owner = MeshComp->GetOwner())
	{
		if (UAdvancedWeaponManager* weaponManager = owner->FindComponentByClass<UAdvancedWeaponManager>())
		{
			weaponManager->AttachHand(weaponManager->SavedGuid, VisualIndex);
		}
	}
}

FString UAttachCurrentWeaponToHand::GetNotifyName_Implementation() const
{
	return TEXT("Attach current weapon to hand");
}
