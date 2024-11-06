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
			// auto world = MeshComp->GetWorld();
			// auto mode = (GEngine->GetNetMode(world) == NM_Client)
			// 				? TEXT("[Client] ")
			// 				: (GEngine->GetNetMode(world) == NM_ListenServer)
			// 				? TEXT("[ListenServer] ")
			// 				: (GEngine->GetNetMode(world) == NM_DedicatedServer)
			// 				? TEXT("[DedicatedServer] ")
			// 				: TEXT("[Standalone]");
			// TRACE(LogWeapon, "%s Attach to BACK notify, saved guid: '%s'", mode, *weaponManager->SavedGuid);
			weaponManager->AttachBack(weaponManager->SavedGuid, VisualIndex);
		}
	}
}

FString UAttachSavedWeaponToBody::GetNotifyName_Implementation() const
{
	return TEXT("Attach saved weapon to body");
}
