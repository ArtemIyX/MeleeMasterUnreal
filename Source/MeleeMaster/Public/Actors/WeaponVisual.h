// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponVisual.generated.h"

/**
 * @brief Visual representation of a weapon in the game. 
 * Manages mesh, sockets, and replication for multiplayer.
 */
UCLASS()
class MELEEMASTER_API AWeaponVisual : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor. Sets default values for this actor's properties.
	 */
	AWeaponVisual();

protected:
	/**
	 * @brief Base component for the weapon, used as a root component.
	 * This component serves as the foundation upon which other components, like the mesh, are attached.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Components")
	USceneComponent* Base;

	/**
	 * @brief Skeletal mesh component for rendering the weapon.
	 * This component holds the weapon's mesh and animations.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/**
	 * @brief Socket name for attaching the weapon to the character's hand.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Sockets")
	FName HandSocket;

	/**
	 * @brief Socket name for attaching the weapon to the character's back.
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="WeaponVisual|Sockets")
	FName BackSocket;

	/**
	 * @brief Unique ID for multiplayer replication.
	 * This is used to ensure each weapon instance can be properly identified in a multiplayer setting.
	 */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Guid, BlueprintReadOnly, Category="WeaponVisual|Replicated")
	FString WeaponGuid;

protected:
	/**
	 * @brief Called when the weapon GUID is replicated.
	 * This function is triggered automatically when the GUID is updated on clients.
	 */
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
	/**
	 * @brief Retrieves the visual index of the weapon.
	 * @return The visual index as an integer.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	int32 GetVisualIndex() const;

	/**
	 * @brief Returns the weapon's unique GUID as a string.
	 * @return The weapon's GUID string.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FString GetGUIDString() const { return WeaponGuid; }

	/**
	 * @brief Sets the weapon's GUID.
	 * @param InGuid The new GUID to set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="WeaponVisual")
	void SetGuidString(FString InGuid);

	/**
	 * @brief Returns the skeletal mesh component of the weapon.
	 * @return The skeletal mesh component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }

	/**
	 * @brief Gets the socket name for attaching the weapon to a character's hand.
	 * @return The hand socket name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FName GetHandSocket() const { return HandSocket; }

	/**
	 * @brief Gets the socket name for attaching the weapon to the character's back.
	 * @return The back socket name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="WeaponVisual")
	FORCEINLINE FName GetBackSocket() const { return BackSocket; }
};
