// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/BasicProjectile.h"


// Sets default values
ABasicProjectile::ABasicProjectile(): DamageCauser(nullptr), Dmg(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicProjectile::Run_Implementation(AActor* InInstigator, float InDamage, FVector Velocity, float Speed)
{
	DamageCauser = InInstigator;
	Dmg = InDamage;
}


