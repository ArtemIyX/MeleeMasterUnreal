// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/LongRangeWeapon.h"

#include "MeleeMaster.h"
#include "Actors/ArrowVisual.h"
#include "Data/RangeWeaponDataAsset.h"
#include "Data/WeaponDataAsset.h"
#include "Subsystems/LoggerLib.h"

ULongRangeWeapon::ULongRangeWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer),
	LocalArrowVisual(nullptr)
{
}

URangeWeaponDataAsset* ULongRangeWeapon::GetRangeData() const
{
	return Cast<URangeWeaponDataAsset>(this->GetData());
}

void ULongRangeWeapon::ObjectEndPlay()
{
	if (IsValid(LocalArrowVisual))
	{
		LocalArrowVisual->Destroy();
	}
	Super::ObjectEndPlay();
}

void ULongRangeWeapon::ShowOrCreateArrowVisual(USkeletalMeshComponent* ParentComponent)
{
	if (!IsValid(LocalArrowVisual))
	{
		UWorld* world = ParentComponent->GetWorld();
		const FVector loc = ParentComponent->GetComponentLocation();
		const FRotator rot = FRotator::ZeroRotator;
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = ParentComponent->GetOwner();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		URangeWeaponDataAsset* rangeData = GetRangeData();
		if (!rangeData)
		{
			TRACEERROR(LogWeapon, "Invalid data for ULongRangeWeapon. Must be 'URangeWeaponDataAsset'");
			return;
		}
		LocalArrowVisual = world->SpawnActor<AArrowVisual>(rangeData->Arrow.VisualActorClass, loc, rot, spawnParams);
		if (IsValid(LocalArrowVisual))
		{
			const ENetRole role = ParentComponent->GetOwner()->GetLocalRole();
			const bool bIsLocallyControlled = (role == ROLE_AutonomousProxy);

			LocalArrowVisual->AttachToComponent(ParentComponent,
			                                    FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                    bIsLocallyControlled
				                                    ? rangeData->Arrow.HandSocketFP
				                                    : rangeData->Arrow.HandSocketTP);
		}
	}
	if (IsValid(LocalArrowVisual))
	{
		LocalArrowVisual->Show();
	}
}

void ULongRangeWeapon::HideArrowVisual()
{
	if (IsValid(LocalArrowVisual))
	{
		LocalArrowVisual->Hide();
	}
}

AArrowVisual* ULongRangeWeapon::GetLocalArrowVisual() const
{
	return LocalArrowVisual;
}

void ULongRangeWeapon::SetLocalArrowVisual(AArrowVisual* InArrow)
{
	LocalArrowVisual = InArrow;
}

void ULongRangeWeapon::GetArrowLocRot_Implementation(FVector& OutLoc, FRotator& OutRot)
{
	OutLoc = FVector::ZeroVector;
	OutRot = FRotator::ZeroRotator;
}

void ULongRangeWeapon::FireArrow_Implementation(float Power)
{
	// IMPLEMENT
}
