// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/BMPlayerController.h"
#include "Character/BMCharacter.h"
#include "Character/BMPlayerCameraManager.h"

void ABMPlayerController::RestartPawn(APawn* NewPawn)
{
	PossessedCharacter = Cast<ABMBaseCharacter>(NewPawn);
	check(PossessedCharacter);

	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	ABMPlayerCameraManager* CastedMgr = Cast<ABMPlayerCameraManager>(PlayerCameraManager);
	if (CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}

	// Trigger our BP event, mostly used for setting up HUD
	OnRestartPawn(NewPawn);
}
