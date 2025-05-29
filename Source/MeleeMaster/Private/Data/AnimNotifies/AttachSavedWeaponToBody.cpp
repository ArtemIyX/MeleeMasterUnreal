// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/AnimNotifies/AttachSavedWeaponToBody.h"

#include "MeleeMaster.h"
#include "Components/AdvancedWeaponManager.h"
#include "Subsystems/LoggerLib.h"

class AWeaponVisual;
class UAdvancedWeaponManager;

void UAttachSavedWeaponToBody::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* owner = MeshComp->GetOwner())
	{
		if (UAdvancedWeaponManager* weaponManager = owner->FindComponentByClass<UAdvancedWeaponManager>())
		{
			weaponManager->AttachBack(weaponManager->SavedGuid, VisualIndex);
		}
	}
}

FString UAttachSavedWeaponToBody::GetNotifyName_Implementation() const
{
	return TEXT("Attach saved weapon to body");
}
