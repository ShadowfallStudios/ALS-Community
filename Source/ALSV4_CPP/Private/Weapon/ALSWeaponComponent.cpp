// Copyright:       Copyright (C) 2024 ALS Community
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#include "Weapon/ALSWeaponComponent.h"
#include "Weapon/ALSWeapon.h"
#include "Character/ALSBaseCharacter.h"
#include "InputActionValue.h"

UALSWeaponComponent::UALSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UALSWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get owner character
	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());

	// Spawn and equip default weapon if specified
	if (DefaultWeaponClass && OwnerCharacter)
	{
		SpawnAndEquipWeapon(DefaultWeaponClass);
	}
}

void UALSWeaponComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}

	// Bind fire input (you can customize these input names)
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &UALSWeaponComponent::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &UALSWeaponComponent::OnFireReleased);
}

void UALSWeaponComponent::EquipWeapon(AALSWeapon* NewWeapon)
{
	// Unequip current weapon
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = NewWeapon;

	if (!EquippedWeapon || !OwnerCharacter)
	{
		return;
	}

	// Attach weapon to character
	EquippedWeapon->SetOwner(OwnerCharacter);
	EquippedWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		WeaponAttachmentBone
	);

	UE_LOG(LogTemp, Warning, TEXT("Weapon equipped: %s"), *EquippedWeapon->GetName());
}

void UALSWeaponComponent::UnequipWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->StopFiring();
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}
}

void UALSWeaponComponent::OnFirePressed()
{
	if (!EquippedWeapon)
	{
		return;
	}

	bIsFiring = true;
	EquippedWeapon->StartFiring();
}

void UALSWeaponComponent::OnFireReleased()
{
	if (!EquippedWeapon)
	{
		return;
	}

	bIsFiring = false;
	EquippedWeapon->StopFiring();
}

void UALSWeaponComponent::SpawnAndEquipWeapon(TSubclassOf<AALSWeapon> WeaponClass)
{
	if (!WeaponClass || !OwnerCharacter)
	{
		return;
	}

	AALSWeapon* NewWeapon = GetWorld()->SpawnActor<AALSWeapon>(WeaponClass, OwnerCharacter->GetActorTransform());
	EquipWeapon(NewWeapon);
}
