// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Actors/HitRecorder.h"


// Sets default values
AHitRecorder::AHitRecorder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(Base);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK Component"));
	SkeletalMeshComponent->SetupAttachment(Base);
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(SkeletalMeshComponent, ParentSocket);

	bRecording = false;
}

// Called when the game starts or when spawned
void AHitRecorder::BeginPlay()
{
	Super::BeginPlay();
}

void AHitRecorder::Finished()
{
	MakeSnapshot();
	GetWorldTimerManager().ClearTimer(PeriodTimerHandle);
	GetWorldTimerManager().ClearTimer(AnimationTimerHandle);
	bRecording = false;
}

void AHitRecorder::MakeSnapshot()
{
	if (IsValid(SkeletalMeshComponent) && IsValid(Weapon))
	{
		if (IsValid(SkeletalMeshComponent->GetSkeletalMeshAsset()) && IsValid(Weapon->GetSkeletalMeshAsset()))
		{
			FWeaponHitDataElement el;
			el.Start = Weapon->GetSocketLocation(StartSocket) - GetActorLocation();
			el.End = Weapon->GetSocketLocation(EndSocket) - GetActorLocation();
			HitData.Elements.Add(el);

			//DrawDebugLine(GetWorld(), el.Start, el.End, FColor::Red, false, 5.0f, 0, 5.0f);
		}
	}
}

// Called every frame
void AHitRecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHitRecorder::Record()
{
	if (!IsValid(Animation))
		return;

	if (bRecording)
	{
		Finished();
	}

	HitData.Elements.Empty();
	bRecording = true;

	MakeSnapshot();
	GetWorldTimerManager().SetTimer(PeriodTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		MakeSnapshot();
	}), Period, true);


	StartPlayAnim();
}

void AHitRecorder::Abort()
{
	Finished();
	bRecording = false;
}

void AHitRecorder::Draw()
{
	for (int32 i = 0; i < HitData.Elements.Num(); ++i)
	{
		DrawDebugCylinder(GetWorld(),
		                  HitData.Elements[i].Start + GetActorLocation(),
		                  HitData.Elements[i].End + GetActorLocation(),
		                  Radius,
		                  8,
		                  FColor::Red, false,
		                  2.0f, -1, 0.5f);
	}
}

void AHitRecorder::StartPlayAnim()
{
	if (!IsValid(Animation))
	{
		Finished();
		return;
	}
#if WITH_EDITOR
	SkeletalMeshComponent->SetUpdateAnimationInEditor(true);
#endif
	SkeletalMeshComponent->PlayAnimation(Animation, false);
	SkeletalMeshComponent->SetPosition(StartTime, false);

	GetWorldTimerManager().SetTimer(AnimationTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		Finished();
	}), Animation->GetPlayLength(), false);
}
