// Copyright:       Copyright (C) 2024 ALS Community
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#include "Weapon/ALSWeapon.h"
#include "Character/ALSBaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AALSWeapon::AALSWeapon()
{
	PrimaryActorTick.TickInterval = 0.016f; // 60 FPS
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Calculate time between shots from fire rate
	TimeBetweenShots = 60.0f / FireRate;
}

void AALSWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Get the character that owns this weapon
	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
}

void AALSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update fire cooldown
	if (TimeUntilNextShot > 0.0f)
	{
		TimeUntilNextShot -= DeltaTime;
	}

	// Continue firing if bIsFiring is true
	if (bIsFiring && TimeUntilNextShot <= 0.0f)
	{
		Fire();
	}
}

void AALSWeapon::Fire()
{
	// Check if enough time has passed since last shot
	if (TimeUntilNextShot > 0.0f || !OwnerCharacter)
	{
		return;
	}

	// Perform weapon trace
	PerformWeaponTrace();

	// Set cooldown for next shot
	TimeUntilNextShot = TimeBetweenShots;
}

void AALSWeapon::StartFiring()
{
	bIsFiring = true;
	Fire(); // Fire immediately
}

void AALSWeapon::StopFiring()
{
	bIsFiring = false;
}

FVector AALSWeapon::GetMuzzleLocation() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}
	return GetActorLocation();
}

FVector AALSWeapon::GetMuzzleDirection() const
{
	return GetActorForwardVector();
}

void AALSWeapon::PerformWeaponTrace()
{
	if (!OwnerCharacter)
	{
		return;
	}

	FVector TraceStart = GetMuzzleLocation();
	FVector TraceEnd = TraceStart + (GetMuzzleDirection() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Pawn,
		QueryParams
	);

	// Debug visualization (remove in production)
	if (bHit)
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor::Green, false, 0.1f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 8, FColor::Red, false, 0.1f);

		// Apply damage if hit something
		if (HitResult.GetActor())
		{
			ApplyDamage(HitResult.GetActor(), HitResult.ImpactPoint, GetMuzzleDirection());
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 2.0f);
	}
}

void AALSWeapon::ApplyDamage(AActor* HitActor, const FVector& ImpactPoint, const FVector& ShotDirection)
{
	if (!HitActor || !OwnerCharacter)
	{
		return;
	}

	// Don't damage owner
	if (HitActor == OwnerCharacter)
	{
		return;
	}

	// Apply damage using Unreal's damage system
	FPointDamageEvent DamageEvent(BaseDamage, FHitResult(), ShotDirection, nullptr);
	HitActor->TakeDamage(BaseDamage, DamageEvent, OwnerCharacter->GetController(), this);

	UE_LOG(LogTemp, Warning, TEXT("Hit: %s - Damage: %.2f"), *HitActor->GetName(), BaseDamage);
}
