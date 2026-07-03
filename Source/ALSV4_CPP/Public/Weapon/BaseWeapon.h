// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDataStruct.h"
#include "BaseWeapon.generated.h"

class AWeaponCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponFiredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponReloadedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChangedSignature, int32, CurrentAmmo, int32, MaxAmmo);

/**
 * Base weapon class for all firearms
 */
AUCLASS()
class ALSV4_CPP_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Weapon initialization
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitializeWeapon(class AALSBaseCharacter* OwnerCharacter, FWeaponData InWeaponData);

	// Firing
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFiring();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFiring();

	UFUNCTION(Server, Reliable, Category = "Weapon")
	void Server_Fire();

	UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
	void Multicast_Fire(FVector FireLocation, FRotator FireRotation);

	// Reloading
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(Server, Reliable, Category = "Weapon")
	void Server_Reload();

	UFUNCTION(NetMulticast, Reliable, Category = "Weapon")
	void Multicast_Reload();

	// Ammunition
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetMaxAmmo() const { return WeaponData.MaxAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetAmmoCapacity() const { return WeaponData.AmmoCapacity; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool HasAmmo() const { return CurrentAmmo > 0; }

	// Weapon data
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FWeaponData GetWeaponData() const { return WeaponData; }

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FWeaponFiredSignature OnWeaponFired;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FWeaponReloadedSignature OnWeaponReloaded;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FAmmoChangedSignature OnAmmoChanged;

protected:
	// Weapon skeletal mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class USkeletalMeshComponent* WeaponMesh;

	// Owner character
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	class AALSBaseCharacter* OwnerCharacter;

	// Weapon data
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	FWeaponData WeaponData;

	// Ammunition
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int32 StoredAmmo;

	// Firing state
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsFiring = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading = false;

	float LastFireTime = 0.0f;
	float ReloadEndTime = 0.0f;

	// Weapon socket names for attachment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = FName("weapon_r");

	// Fire trace parameters
	void PerformLineTrace(FVector& OutHitLocation, AActor*& OutHitActor);

	// Damage application
	void ApplyDamage(AActor* HitActor, FVector HitLocation, FVector FireDirection);

	// Animation and effects
	virtual void PlayFireAnimation();
	virtual void PlayReloadAnimation();

	void OnReloadComplete();
};
