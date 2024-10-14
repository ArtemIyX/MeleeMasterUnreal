// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponVisual.generated.h"

UCLASS()
class MELEEMASTER_API AWeaponVisual : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponVisual();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Components")
	USceneComponent* Base;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Sockets")
	FName HandSocket;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Sockets")
	FName BackSocket;

	/**
	 * @brief Unique id for multiplayer replication
	 */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Guid, BlueprintReadOnly, Category="WeaponVisual|Replicated")
	FString WeaponGuid;

protected:
	UFUNCTION()
	virtual void OnRep_Guid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	int32 GetVisualIndex() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FString GetGUIDString() const { return WeaponGuid; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="WeaponVisual")
	void SetGuidString(FString InGuid);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FName GetHandSocket() const { return HandSocket; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FName GetBackSocket() const { return BackSocket; }
};
