#pragma once
#include "CoreMinimal.h"
#include "WeaponTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct MELEEMASTER_API FWeaponHitDataElement
{
	GENERATED_BODY()
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