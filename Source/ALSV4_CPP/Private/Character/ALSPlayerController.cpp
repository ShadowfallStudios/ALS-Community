// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Drakynfly


#include "Character/ALSPlayerController.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Character/Animation/ALSPlayerCameraBehavior.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AALSPlayerController::OnPossess(APawn* NewPawn)
{
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	Super::OnPossess(NewPawn);

	// Servers want to setup camera only in listen servers.
	if (!IsRunningDedicatedServer())
	{
		SetupCamera();
	}
}

void AALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	SetupCamera();
}

void AALSPlayerController::SetupCamera()
{
	PossessedCharacter = Cast<AALSBaseCharacter>(GetPawn());
	check(PossessedCharacter);

	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CastedMgr = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void AALSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(FKey("Tab"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleHud);
	InputComponent->BindKey(FKey("V"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleDebugView);
	InputComponent->BindKey(FKey("T"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleTraces);
	InputComponent->BindKey(FKey("Y"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleDebugShapes);
	InputComponent->BindKey(FKey("U"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleLayerColors);
	InputComponent->BindKey(FKey("I"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleCharacterInfo);
	InputComponent->BindKey(FKey("Z"), EInputEvent::IE_Pressed, this, &AALSPlayerController::ToggleSlomo);
}

void AALSPlayerController::ToggleGlobalTimeDilationLocal(float TimeDilation)
{
	if (UKismetSystemLibrary::IsStandalone(this))
	{
		UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
	}
}

void AALSPlayerController::ToggleSlomo()
{
	bSlomo = !bSlomo;
	ToggleGlobalTimeDilationLocal(bSlomo ? 0.15f : 1.f);
}

void AALSPlayerController::ToggleDebugView()
{
	bDebugView = !bDebugView;

	AALSPlayerCameraManager* CamManager = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (CamManager)
	{
		UALSPlayerCameraBehavior* CameraBehavior = Cast<UALSPlayerCameraBehavior>(CamManager->CameraBehavior->GetAnimInstance());
		if (CameraBehavior)
		{
			CameraBehavior->bDebugView = bDebugView;
		}
	}
}
