// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HideSwordTrailAnimNotify.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API UHideSwordTrailAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn, ClampMin=0), Category="SwordTrail")
	int32 VisualIndex{0};
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
							const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;
};
