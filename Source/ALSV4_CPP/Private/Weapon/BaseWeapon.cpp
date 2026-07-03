// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#include "Weapon/BaseWeapon.h"
#include "Character/ALSBaseCharacter.h"
#include "Character/WeaponCharacter.h"
#include "Character/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.TickInterval = 0.016f;
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;

	// Create weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECC_QueryOnly);
	RootComponent = WeaponMesh;

	WeaponSocketName = FName("weapon_r");
	CurrentAmmo = 0;
	StoredAmmo = 0;
	bIsFiring = false;
	bIsReloading = false;
	LastFireTime = 0.0f;
	ReloadEndTime = 0.0f;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority)
	{
		CurrentAmmo = WeaponData.AmmoCapacity;
		StoredAmmo = WeaponData.MaxAmmo - WeaponData.AmmoCapacity;
	}
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (bIsReloading && GetWorld()->TimeSeconds >= ReloadEndTime)
		{
			OnReloadComplete();
		}
	}
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDOReplicateVariable(ABaseWeapon, WeaponData);
	FDOReplicateVariable(ABaseWeapon, CurrentAmmo);
	FDOReplicateVariable(ABaseWeapon, StoredAmmo);
	FDOReplicateVariable(ABaseWeapon, bIsReloading);
	FDOReplicateVariable(ABaseWeapon, OwnerCharacter);
}

void ABaseWeapon::InitializeWeapon(AALSBaseCharacter* InOwnerCharacter, FWeaponData InWeaponData)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	OwnerCharacter = InOwnerCharacter;
	WeaponData = InWeaponData;
	CurrentAmmo = WeaponData.AmmoCapacity;
	StoredAmmo = WeaponData.MaxAmmo - WeaponData.AmmoCapacity;
}

void ABaseWeapon::StartFiring()
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_Fire();
	}

	bIsFiring = true;
}

void ABaseWeapon::StopFiring()
{
	bIsFiring = false;
}

void ABaseWeapon::Server_Fire_Implementation()
{
	if (!OwnerCharacter || !OwnerCharacter->IsValid())
	{
		return;
	}

	if (GetWorld()->TimeSeconds - LastFireTime < WeaponData.FireRate)
	{
		return;
	}

	if (CurrentAmmo <= 0)
	{
		return;
	}

	if (bIsReloading)
	{
		return;
	}

	LastFireTime = GetWorld()->TimeSeconds;
	CurrentAmmo--;

	FVector FireLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.0f;
	FRotator FireRotation = OwnerCharacter->GetActorRotation();

	Multicast_Fire(FireLocation, FireRotation);

	OnAmmoChanged.Broadcast(CurrentAmmo, WeaponData.MaxAmmo);
}

void ABaseWeapon::Multicast_Fire_Implementation(FVector FireLocation, FRotator FireRotation)
{
	PlayFireAnimation();

	for (int32 i = 0; i < WeaponData.BulletsPerShot; ++i)
	{
		FRotator SpreadRotation = FireRotation;
		if (WeaponData.Spread > 0.0f)
		{
			SpreadRotation.Pitch += FMath::RandRange(-WeaponData.Spread, WeaponData.Spread);
			SpreadRotation.Yaw += FMath::RandRange(-WeaponData.Spread, WeaponData.Spread);
		}

		FVector TraceStart = FireLocation;
		FVector TraceEnd = TraceStart + SpreadRotation.Vector() * WeaponData.Range;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerCharacter);
		QueryParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Pawn, QueryParams);

		if (bHit && HitResult.GetActor())
		{
			if (GetOwnerRole() == ROLE_Authority)
			{
				ApplyDamage(HitResult.GetActor(), HitResult.ImpactPoint, SpreadRotation.Vector());
			}
		}
	}

	OnWeaponFired.Broadcast();
}

void ABaseWeapon::Reload()
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_Reload();
	}
}

void ABaseWeapon::Server_Reload_Implementation()
{
	if (bIsReloading || CurrentAmmo == WeaponData.AmmoCapacity)
	{
		return;
	}

	bIsReloading = true;
	ReloadEndTime = GetWorld()->TimeSeconds + WeaponData.ReloadTime;

	Multicast_Reload();
}

void ABaseWeapon::Multicast_Reload_Implementation()
{
	PlayReloadAnimation();
}

void ABaseWeapon::OnReloadComplete()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	int32 AmmoNeeded = WeaponData.AmmoCapacity - CurrentAmmo;
	int32 AmmoToLoad = FMath::Min(AmmoNeeded, StoredAmmo);

	CurrentAmmo += AmmoToLoad;
	StoredAmmo -= AmmoToLoad;
	bIsReloading = false;

	OnWeaponReloaded.Broadcast();
	OnAmmoChanged.Broadcast(CurrentAmmo, WeaponData.MaxAmmo);
}

void ABaseWeapon::ApplyDamage(AActor* HitActor, FVector HitLocation, FVector FireDirection)
{
	if (!HitActor || !OwnerCharacter)
	{
		return;
	}

	UHealthComponent* TargetHealth = HitActor->FindComponentByClass<UHealthComponent>();
	if (TargetHealth)
	{
		TargetHealth->Server_TakeDamage(WeaponData.Damage, OwnerCharacter);
	}
	else
	{
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(WeaponData.Damage, DamageEvent, OwnerCharacter->GetController(), OwnerCharacter);
	}
}

void ABaseWeapon::PlayFireAnimation()
{
	if (WeaponMesh)
	{
		// Play fire montage if available
	}
}

void ABaseWeapon::PlayReloadAnimation()
{
	if (WeaponMesh)
	{
		// Play reload montage if available
	}
}
