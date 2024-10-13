﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/WeaponVisual.h"
#include "Components/AdvancedWeaponManager.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
AWeaponVisual::AWeaponVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	Base->SetComponentTickEnabled(false);
	SetRootComponent(Base);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(Base);
	SkeletalMeshComponent->SetComponentTickEnabled(false);

	bReplicates = true;

	HandSocket = FName(TEXT("None"));
	BackSocket = FName(TEXT("None"));
}

void AWeaponVisual::OnRep_Guid()
{
}

// Called when the game starts or when spawned
void AWeaponVisual::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* owner = GetOwner())
	{
		if (UAdvancedWeaponManager* manager = owner->FindComponentByClass<UAdvancedWeaponManager>())
		{
			manager->AttachBack(this);
		}
	}
}

// Called every frame
void AWeaponVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponVisual::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(AWeaponVisual, WeaponGuid, Params);
}

void AWeaponVisual::SetGuidString(FString InGuid)
{
	this->WeaponGuid = InGuid;
	MARK_PROPERTY_DIRTY_FROM_NAME(AWeaponVisual, WeaponGuid, this);
}
