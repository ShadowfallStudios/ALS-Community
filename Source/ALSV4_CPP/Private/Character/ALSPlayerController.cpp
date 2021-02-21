// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    Drakynfly


#include "Character/ALSPlayerController.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Components/ALSDebugComponent.h"
#include "Kismet/GameplayStatics.h"

void AALSPlayerController::OnPossess(APawn* NewPawn)
{
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	Super::OnPossess(NewPawn);

	// Servers want to setup camera only in listen servers.
	if (!IsRunningDedicatedServer())
	{
		SetupCamera();
	}

	// Bind inputs for debugging
	UActorComponent* Comp = NewPawn->GetComponentByClass(UALSDebugComponent::StaticClass());
	if (Comp)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(Comp);
		InputComponent->BindKey(FKey("Tab"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleHud);
		InputComponent->BindKey(FKey("V"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleDebugView);
		InputComponent->BindKey(FKey("T"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleTraces);
		InputComponent->BindKey(FKey("Y"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleDebugShapes);
		InputComponent->BindKey(FKey("U"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleLayerColors);
		InputComponent->BindKey(FKey("I"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleCharacterInfo);
		InputComponent->BindKey(FKey("Z"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleSlomo);
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
}
