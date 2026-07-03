// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#include "Character/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
	SetIsReplicatedEnabled(true);

	MaxHealth = 100.0f;
	CurrentHealth = 100.0f;
	bIsDead = false;
	BloodEffectRadius = 100.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDOReplicateVariable(UHealthComponent, CurrentHealth);
}

void UHealthComponent::TakeDamage(float DamageAmount, AActor* DamageInstigator)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_TakeDamage(DamageAmount, DamageInstigator);
	}
}

void UHealthComponent::Server_TakeDamage_Implementation(float DamageAmount, AActor* DamageInstigator)
{
	if (bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	Multicast_OnDamaged(DamageAmount, GetOwner()->GetActorLocation());

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, DamageInstigator);

	if (CurrentHealth <= 0.0f)
	{
		Server_Die(DamageInstigator);
	}
}

void UHealthComponent::Multicast_OnDamaged_Implementation(float DamageAmount, FVector DamageLocation)
{
	SpawnBloodEffect(DamageLocation);
}

void UHealthComponent::Heal(float HealAmount)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_Heal(HealAmount);
	}
}

void UHealthComponent::Server_Heal_Implementation(float HealAmount)
{
	if (bIsDead)
	{
		return;
	}

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, nullptr);
}

void UHealthComponent::Die(AActor* KillerActor)
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_Die(KillerActor);
	}
}

void UHealthComponent::Server_Die_Implementation(AActor* KillerActor)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	CurrentHealth = 0.0f;

	Multicast_Die(KillerActor);
}

void UHealthComponent::Multicast_Die_Implementation(AActor* KillerActor)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		OwnerCharacter->SetActorEnableCollision(false);
	}

	OnDeath.Broadcast(GetOwner(), KillerActor);
}

void UHealthComponent::SpawnBloodEffect(FVector Location)
{
	if (BloodFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodFX, Location);
	}
}
