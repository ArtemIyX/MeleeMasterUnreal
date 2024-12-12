// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicProjectile.generated.h"

UCLASS()
class MELEEMASTER_API ABasicProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasicProjectile();
protected:
	UPROPERTY(BlueprintReadWrite)
	AActor* DamageCauser;

	UPROPERTY(BlueprintReadWrite, DisplayName="Damage")
	float Dmg;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintAuthorityOnly)
	void Run(AActor* InInstigator, float InDamage, FVector Velocity, float Speed);
};
