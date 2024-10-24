﻿#pragma once
#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponHitDataElement
{
	GENERATED_BODY()

public:
	FWeaponHitDataElement(): Start(0.0f), End(0.0f)
	{
	}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector End;
};

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponHitData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FWeaponHitDataElement> Elements;
};

UENUM(Blueprintable, BlueprintType)
enum class EWeaponTier : uint8
{
	Light,
	Medium,
	High
};

UENUM(Blueprintable, BlueprintType)
enum class EDamageReturn : uint8
{
	Failed,
	Alive,
	Dead
};