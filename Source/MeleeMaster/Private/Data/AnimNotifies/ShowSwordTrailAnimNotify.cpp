// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Data/AnimNotifies/ShowSwordTrailAnimNotify.h"

#include "Components/AdvancedWeaponManager.h"

void UShowSwordTrailAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (AActor* owner = MeshComp->GetOwner())
	{
		if (UAdvancedWeaponManager* weaponManager = owner->FindComponentByClass<UAdvancedWeaponManager>())
		{
			weaponManager->ShowWeaponTrail(VisualIndex);
		}
	}
}

FString UShowSwordTrailAnimNotify::GetNotifyName_Implementation() const
{
	return FString(TEXT("Show Trail"));
}
