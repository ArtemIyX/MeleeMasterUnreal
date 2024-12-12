// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/ArrowVisual.h"


// Sets default values
AArrowVisual::AArrowVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponentMain = CreateDefaultSubobject<USceneComponent>(TEXT("Main"));
	SetRootComponent(SceneComponentMain);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	SkeletalMeshComponent->SetupAttachment(SceneComponentMain);
}

// Called when the game starts or when spawned
void AArrowVisual::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArrowVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrowVisual::SetSkeletal(USkeletalMesh* InMesh)
{
	SkeletalMeshComponent->SetSkeletalMesh(InMesh);

}

void AArrowVisual::Show()
{
	SkeletalMeshComponent->CastShadow = true;
	SkeletalMeshComponent->bCastHiddenShadow = true;
	SkeletalMeshComponent->SetVisibility(true);
	SkeletalMeshComponent->SetHiddenInGame(false);
	this->SetHidden(false);
}

void AArrowVisual::Hide()
{
	SkeletalMeshComponent->CastShadow = false;
	SkeletalMeshComponent->bCastHiddenShadow = false;
	SkeletalMeshComponent->SetVisibility(false);
	SkeletalMeshComponent->SetHiddenInGame(true);
	this->SetHidden(true);
}
