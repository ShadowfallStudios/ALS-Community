// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#include "Weapon/WeaponTypes.h"

AM9Pistol::AM9Pistol()
{
	WeaponData.WeaponType = TEXT("Pistol");
	WeaponData.Damage = 20.0f;
	WeaponData.FireRate = 0.15f;
	WeaponData.AmmoCapacity = 15;
	WeaponData.MaxAmmo = 120;
	WeaponData.ReloadTime = 1.5f;
	WeaponData.Range = 5000.0f;
	WeaponData.Spread = 3.0f;
	WeaponData.BulletsPerShot = 1;
}

void AM9Pistol::BeginPlay()
{
	Super::BeginPlay();
}

AM4A1Rifle::AM4A1Rifle()
{
	WeaponData.WeaponType = TEXT("Rifle");
	WeaponData.Damage = 35.0f;
	WeaponData.FireRate = 0.1f;
	WeaponData.AmmoCapacity = 30;
	WeaponData.MaxAmmo = 300;
	WeaponData.ReloadTime = 2.5f;
	WeaponData.Range = 10000.0f;
	WeaponData.Spread = 2.0f;
	WeaponData.BulletsPerShot = 1;
}

void AM4A1Rifle::BeginPlay()
{
	Super::BeginPlay();
}
