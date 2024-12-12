// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowVisual.generated.h"

UCLASS()
class MELEEMASTER_API AArrowVisual : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrowVisual();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* SceneComponentMain;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSkeletal(USkeletalMesh* InMesh);

	void Show();
	void Hide();
};
