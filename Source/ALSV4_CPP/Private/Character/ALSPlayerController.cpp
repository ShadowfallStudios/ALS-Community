// Copyright (C) 2020, Doga Can Yanikoglu


#include "Character/ALSPlayerController.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"

void AALSPlayerController::OnRestartPawn(APawn* NewPawn)
{
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	check(PossessedCharacter);

	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CastedMgr = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}
