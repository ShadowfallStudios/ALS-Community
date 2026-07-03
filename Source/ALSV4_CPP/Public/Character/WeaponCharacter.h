// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "Weapon/WeaponDataStruct.h"
#include "WeaponCharacter.generated.h"

class ABaseWeapon;
class UHealthComponent;

/**
 * Character class with weapon and combat capabilities
 */
AUCLASS()
class ALSV4_CPP_API AWeaponCharacter : public AALSCharacter
{
	GENERATED_BODY()

public:
	AWeaponCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Weapon management
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(ABaseWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UnequipWeapon();

	// Combat actions - input callbacks
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ReloadWeapon();

	// Health component access
	UFUNCTION(BlueprintCallable, Category = "Combat")
	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	// Health component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UHealthComponent* HealthComponent;

	// Current equipped weapon
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	ABaseWeapon* CurrentWeapon;

	// Weapon attachment socket
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	FName WeaponAttachSocket = FName("weapon_r");

	// Combat state
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsInCombat = false;

	// Take damage override
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
