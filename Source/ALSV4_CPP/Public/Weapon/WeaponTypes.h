// Copyright:       Copyright (C) 2024 Combat System
// Source Code:     https://github.com/Klebold2009/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "WeaponTypes.generated.h"

/**
 * M9 Pistol
 */
UCLASS()
class ALSV4_CPP_API AM9Pistol : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AM9Pistol();

	virtual void BeginPlay() override;
};

/**
 * M4A1 Assault Rifle
 */
UCLASS()
class ALSV4_CPP_API AM4A1Rifle : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AM4A1Rifle();

	virtual void BeginPlay() override;
};
