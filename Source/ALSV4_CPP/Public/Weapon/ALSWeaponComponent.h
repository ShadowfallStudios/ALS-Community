// Copyright:       Copyright (C) 2024 ALS Community
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSWeaponComponent.generated.h"

class AALSWeapon;
class AALSBaseCharacter;

UCLASS(ClassGroup = (ALS, Weapon), meta = (BlueprintSpawnableComponent))
class ALSV4_CPP_API UALSWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UALSWeaponComponent();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	/** Equip a weapon */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void EquipWeapon(AALSWeapon* NewWeapon);

	/** Unequip current weapon */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void UnequipWeapon();

	/** Get currently equipped weapon */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	AALSWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	/** Fire callback */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void OnFirePressed();

	/** Stop firing callback */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void OnFireReleased();

	/** Spawn and equip weapon from class */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void SpawnAndEquipWeapon(TSubclassOf<AALSWeapon> WeaponClass);

public:
	/** Default weapon class to spawn on begin play */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	TSubclassOf<AALSWeapon> DefaultWeaponClass = nullptr;

	/** Attachment bone name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	FName WeaponAttachmentBone = TEXT("VB RHS_ik_hand_gun");

private:
	/** Currently equipped weapon */
	UPROPERTY()
	AALSWeapon* EquippedWeapon = nullptr;

	/** Owner character */
	UPROPERTY()
	AALSBaseCharacter* OwnerCharacter = nullptr;

	/** Is currently firing */
	bool bIsFiring = false;
};
