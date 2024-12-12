// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "LongRangeWeapon.generated.h"

class AArrowVisual;
class URangeWeaponDataAsset;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API ULongRangeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()
public:
	ULongRangeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	UPROPERTY(Transient)
	AArrowVisual* LocalArrowVisual;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	URangeWeaponDataAsset* GetRangeData() const;

public:

	virtual void ObjectEndPlay() override;
	
	virtual void ShowOrCreateArrowVisual(USkeletalMeshComponent* ParentComponent);
	virtual void HideArrowVisual();
	
	virtual AArrowVisual* GetLocalArrowVisual() const;
	virtual void SetLocalArrowVisual(AArrowVisual* InArrow);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintNativeEvent)
	void GetArrowLocRot(FVector& OutLoc, FRotator& OutRot);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintNativeEvent)
	void FireArrow(
		float Power);
};
