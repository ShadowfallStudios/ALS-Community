// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#include "Character/WeaponCharacter.h"
#include "Character/HealthComponent.h"
#include "Weapon/BaseWeapon.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

AWeaponCharacter::AWeaponCharacter()
{
	PrimaryActorTick.TickInterval = 0.016f;

	// Create health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AWeaponCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Fire input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// TODO: Bind fire input actions here if using Enhanced Input System
	}
}

void AWeaponCharacter::EquipWeapon(ABaseWeapon* NewWeapon)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (CurrentWeapon)
	{
		UnequipWeapon();
	}

	CurrentWeapon = NewWeapon;

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponAttachSocket);
		
		FWeaponData WeaponData;
		CurrentWeapon->InitializeWeapon(this, WeaponData);
	}
}

void AWeaponCharacter::UnequipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void AWeaponCharacter::FireWeapon()
{
	if (CurrentWeapon && !bIsDead)
	{
		CurrentWeapon->StartFiring();
	}
}

void AWeaponCharacter::StopFireWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

void AWeaponCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

float AWeaponCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->Server_TakeDamage(Damage, DamageCauser);
	}

	return Damage;
}
