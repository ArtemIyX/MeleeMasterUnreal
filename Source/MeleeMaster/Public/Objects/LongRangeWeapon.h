// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "AbstractWeapon.h"
#include "LongRangeWeapon.generated.h"

class AArrowVisual;
class URangeWeaponDataAsset;
/**
 * @brief Base class for long-range weapons in the MeleeMaster plugin.
 * 
 * Extends UAbstractWeapon to provide functionality for managing arrow visuals and firing mechanics.
 * This class is Blueprintable and can be used as a type in Blueprints.
 */
UCLASS(Blueprintable, BlueprintType)
class MELEEMASTER_API ULongRangeWeapon : public UAbstractWeapon
{
	GENERATED_BODY()

#pragma region Constructor
public:
	/**
	 * @brief Constructor for the ULongRangeWeapon class.
	 * @param ObjectInitializer Object initializer for setting default values.
	 */
	ULongRangeWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
#pragma endregion Constructor

#pragma region Properties
protected:
	/**
	 * @brief Local arrow visual component for this weapon.
	 * @note Transient, not replicated.
	 */
	UPROPERTY(Transient)
	AArrowVisual* LocalArrowVisual;
#pragma endregion Properties

#pragma region Overrides
public:
	/**
	 * @brief Called when the object is removed from the world.
	 * Cleans up resources associated (@see LocalArrowVisual) with the long-range weapon.@
	 */
	virtual void ObjectEndPlay() override;
#pragma endregion Overrides

#pragma region Data
public:
	/**
	 * @brief Retrieves the range weapon data asset.
	 * @return The range weapon data asset.
	 * @note Cast<URangeWeaponDataAsset>(this->GetData())
	 * @see UAbstractWeapon::GetData()
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	URangeWeaponDataAsset* GetRangeData() const;
	
	virtual float GetTotalDamagePerDirection(EWeaponDirection WeaponDirection) const override;
#pragma endregion Data

#pragma region Arrow Visual
public:
	/**
	 * @brief Shows or creates the arrow visual component.
	 * @param ParentComponent The skeletal mesh component to attach the arrow visual to.
	 */
	virtual void ShowOrCreateArrowVisual(USkeletalMeshComponent* ParentComponent);

	/**
	 * @brief Hides the arrow visual component.
	 */
	virtual void HideArrowVisual();

	/**
	 * @brief Gets the local arrow visual component.
	 * @return The local arrow visual component.
	 */
	virtual AArrowVisual* GetLocalArrowVisual() const;

	/**
	 * @brief Sets the local arrow visual component.
	 * @param InArrow The new arrow visual component to set.
	 */
	virtual void SetLocalArrowVisual(AArrowVisual* InArrow);
#pragma endregion Arrow Visual

#pragma region Arrow Mechanics
public:
	/**
	 * @brief Retrieves the location and rotation for the arrow.
	 * @param OutLoc The output location for the arrow.
	 * @param OutRot The output rotation for the arrow.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints, and BlueprintAuthorityOnly, meaning it can only be called on the server.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintNativeEvent)
	void GetArrowLocRot(FVector& OutLoc, FRotator& OutRot);

	/**
	 * @brief Fires an arrow with the specified power.
	 * @param Power The power level of the arrow shot.
	 * This function is a BlueprintNativeEvent, allowing it to be overridden in Blueprints, and BlueprintAuthorityOnly, meaning it can only be called on the server.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintNativeEvent)
	void FireArrow(float Power);
#pragma endregion Arrow Mechanics
};
