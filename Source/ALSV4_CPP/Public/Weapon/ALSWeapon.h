// Copyright:       Copyright (C) 2024 ALS Community
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALSWeapon.generated.h"

class AALSWeapon : public AActor
{
	GENERATED_BODY()

public:
	AALSWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Fire the weapon */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void Fire();

	/** Start continuous firing */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void StartFiring();

	/** Stop continuous firing */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	void StopFiring();

	/** Get if weapon is currently firing */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	bool IsFiring() const { return bIsFiring; }

	/** Get weapon muzzle location */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	FVector GetMuzzleLocation() const;

	/** Get weapon forward direction */
	UFUNCTION(BlueprintCallable, Category = "ALS|Weapon")
	FVector GetMuzzleDirection() const;

public:
	/** Weapon mesh component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Components")
	class UStaticMeshComponent* WeaponMesh = nullptr;

	/** Muzzle socket name for bullet spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** Fire rate - shots per minute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	float FireRate = 600.0f;

	/** Damage per shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	float BaseDamage = 30.0f;

	/** Trace distance for bullet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Weapon|Settings")
	float TraceDistance = 5000.0f;

	/** Weapon owner character */
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Weapon")
	class AALSBaseCharacter* OwnerCharacter = nullptr;

private:
	/** Time between shots */
	float TimeBetweenShots = 0.1f;

	/** Time until next shot is allowed */
	float TimeUntilNextShot = 0.0f;

	/** Is weapon currently firing */
	bool bIsFiring = false;

	/** Perform line trace and apply damage */
	void PerformWeaponTrace();

	/** Apply damage to hit actor */
	void ApplyDamage(AActor* HitActor, const FVector& ImpactPoint, const FVector& ShotDirection);
};
