﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


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

void UAbstractWeapon::OnRep_Visuals()
{
	
}

void UAbstractWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAbstractWeapon, Data, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAbstractWeapon, Visuals, Params);
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

void UAbstractWeapon::SetVisual(const TArray<AWeaponVisual*>& InVisuals)
{
	Visuals = InVisuals;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAbstractWeapon, Visuals, this);
}

void UAbstractWeapon::ClearVisual()
{
	Visuals.Empty();
	MARK_PROPERTY_DIRTY_FROM_NAME(UAbstractWeapon, Visuals, this);
}

void UAbstractWeapon::SetVisualActor(int32 InIndex, AWeaponVisual* InVisual)
{
	if (Visuals.IsValidIndex(InIndex))
	{
		Visuals[InIndex] = InVisual;
	}
	else
	{
		Visuals.Add(InVisual);
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UAbstractWeapon, Visuals, this);
}

AWeaponVisual* UAbstractWeapon::RemoveVisualActor(int32 Index)
{
	if (Visuals.IsValidIndex(Index))
	{
		AWeaponVisual* res = Visuals[Index];
		Visuals.RemoveAt(Index);
		MARK_PROPERTY_DIRTY_FROM_NAME(UAbstractWeapon, Visuals, this);
		return res;
	}
	return nullptr;
}

void UAbstractWeapon::GetVisual(TArray<AWeaponVisual*>& OutVisual)
{
	OutVisual = this->Visuals;
}

bool UAbstractWeapon::GetVisualActor(int32 Index, AWeaponVisual*& OutVisual)
{
	OutVisual = nullptr;
	if (Visuals.IsValidIndex(Index))
	{
		OutVisual = Visuals[Index];
		return IsValid(OutVisual);
	}
	return false;
}
