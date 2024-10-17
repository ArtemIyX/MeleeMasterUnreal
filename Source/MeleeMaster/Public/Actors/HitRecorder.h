// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "WeaponTypes.h"
#include "GameFramework/Actor.h"
#include "HitRecorder.generated.h"

UCLASS()
class MELEEMASTER_API AHitRecorder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHitRecorder();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Construct")
	USceneComponent* Base;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Construct")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Construct")
	USkeletalMeshComponent* Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Construct")
	FName ParentSocket{"SOCK_LongSword_Hand"};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	FTimerHandle PeriodTimerHandle;

	UPROPERTY()
	FTimerHandle AnimationTimerHandle;

	UFUNCTION(BlueprintCallable)
	virtual void Finished();

	UFUNCTION(BlueprintCallable)
	virtual void MakeSnapshot();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="HitRecorder")
	void Record();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="HitRecorder")
	void Abort();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="HitRecorder")
	void Draw();

	void StartPlayAnim();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Draw")
	float Radius{10.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	UAnimSequence* Animation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	float PlayLength{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	float StartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	float SnapshotFrequency{0.05f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	float AnimPlayRate{1.0f};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	FName StartSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitRecorder|Setup")
	FName EndSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, CallInEditor, Category="HitRecorder|Data")
	FWeaponHitData HitData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="HitRecorder|Data")
	bool bRecording{false};
};
