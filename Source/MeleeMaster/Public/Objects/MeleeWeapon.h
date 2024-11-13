// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "MeleeWeapon.generated.h"

struct FMeleeCombinedData;
class UMeleeWeaponDataAsset;
/**
 * 
 */
UCLASS(Blueprintable, Abstract, BlueprintType)
class MELEEMASTER_API UMeleeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()

public:
	UMeleeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldEquipped)
	uint8 bShieldEquipped;


protected:
	UFUNCTION()
	void OnRep_ShieldEquipped();

public:
	void SetShieldEquipped(bool bInValue);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponShieldSupported() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsShieldEquipped() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMeleeWeaponDataAsset* GetMeleeData() const;

	const FMeleeCombinedData& GetCurrentMeleeCombinedData() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
