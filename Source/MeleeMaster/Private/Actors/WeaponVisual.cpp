// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/WeaponVisual.h"
#include "Components/AdvancedWeaponManager.h"

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
