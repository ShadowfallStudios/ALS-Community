// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthChangedSignature, float, NewHealth, float, MaxHealth, AActor*, DamageInstigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathSignature, AActor*, DeadActor, AActor*, KillerActor);

/**
 * Health component for characters - handles damage and death
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSV4_CPP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Damage handling
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount, AActor* DamageInstigator = nullptr);

	UFUNCTION(Server, Reliable)
	void Server_TakeDamage(float DamageAmount, AActor* DamageInstigator);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDamaged(float DamageAmount, FVector DamageLocation);

	// Healing
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(Server, Reliable)
	void Server_Heal(float HealAmount);

	// Health queries
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	// Death handling
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die(AActor* KillerActor = nullptr);

	UFUNCTION(Server, Reliable)
	void Server_Die(AActor* KillerActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Die(AActor* KillerActor);

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FDeathSignature OnDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bIsDead;

	// Damage effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Effects")
	float BloodEffectRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Effects")
	class UParticleSystem* BloodFX;

	void SpawnBloodEffect(FVector Location);
};
