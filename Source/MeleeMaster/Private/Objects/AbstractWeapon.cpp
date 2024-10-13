// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/AbstractWeapon.h"

#include "Actors/WeaponVisual.h"
#include "Data/WeaponDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


FName UAbstractWeapon::FirstHandSlot = FName(TEXT("First"));
FName UAbstractWeapon::SecondHandSlot = FName(TEXT("Second"));

UAbstractWeapon::UAbstractWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), Data(nullptr)
{
}

void UAbstractWeapon::OnRep_Data()
{
}

void UAbstractWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAbstractWeapon, Data, Params);
}

bool UAbstractWeapon::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}

bool UAbstractWeapon::IsValidData() const
{
	return IsValid(GetData());
}

void UAbstractWeapon::SetData(UWeaponDataAsset* InData)
{
	this->Data = InData;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAbstractWeapon, Data, this);
}

void UAbstractWeapon::SetVisual(const TMap<FName, AWeaponVisual*>& InVisuals)
{
	Visuals = InVisuals;
}

void UAbstractWeapon::ClearVisual()
{
	Visuals.Empty();
}

void UAbstractWeapon::SetVisualActor(FName Index, AWeaponVisual* InVisual)
{
	if (Visuals.Contains(Index))
	{
		Visuals[Index] = InVisual;
	}
	else
	{
		Visuals.Add(Index, InVisual);
	}
}

AWeaponVisual* UAbstractWeapon::RemoveVisualActor(FName Index)
{
	if (Visuals.Contains(Index))
	{
		AWeaponVisual* res = Visuals[Index];
		Visuals.Remove(Index);
		return res;
	}
	return nullptr;
}

void UAbstractWeapon::GetVisuals(TMap<FName, AWeaponVisual*>& OutVisual)
{
	OutVisual = this->Visuals;
}

void UAbstractWeapon::GetVisual(TArray<AWeaponVisual*>& OutVisual)
{
	this->Visuals.GenerateValueArray(OutVisual);
}

bool UAbstractWeapon::GetVisualActor(FName Index, AWeaponVisual*& OutVisual)
{
	OutVisual = nullptr;
	if (Visuals.Contains(Index))
	{
		OutVisual = Visuals[Index];
		return IsValid(OutVisual);
	}
	return false;
}
