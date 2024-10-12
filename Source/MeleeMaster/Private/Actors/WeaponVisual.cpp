// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/WeaponVisual.h"


// Sets default values
AWeaponVisual::AWeaponVisual()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

