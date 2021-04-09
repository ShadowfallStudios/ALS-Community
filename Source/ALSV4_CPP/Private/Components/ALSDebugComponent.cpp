// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    

#include "Components/ALSDebugComponent.h"


#include "Character/ALSBaseCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Character/Animation/ALSPlayerCameraBehavior.h"
#include "Kismet/GameplayStatics.h"

bool UALSDebugComponent::bDebugView = false;
bool UALSDebugComponent::bShowTraces = false;
bool UALSDebugComponent::bShowDebugShapes = false;
bool UALSDebugComponent::bShowLayerColors = false;

UALSDebugComponent::UALSDebugComponent()
{
#if UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = false;
#else
	PrimaryComponentTick.bCanEverTick = true;
#endif
}

void UALSDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	if (!OwnerCharacter)
	{
		return;
	}

	if (bNeedsColorReset)
	{
		bNeedsColorReset = false;
		SetResetColors();
	}

	if (bShowLayerColors)
	{
		UpdateColoringSystem();
	}
	else
	{
		bNeedsColorReset = true;
	}

	if (bShowDebugShapes)
	{
		DrawDebugSpheres();

		APlayerController* Controller = Cast<APlayerController>(OwnerCharacter->GetController());
		if (Controller)
		{
			AALSPlayerCameraManager* CamManager = Cast<AALSPlayerCameraManager>(Controller->PlayerCameraManager);
			if (CamManager)
			{
				CamManager->DrawDebugTargets(OwnerCharacter->GetThirdPersonPivotTarget().GetLocation());
			}
		}
	}
#endif
}

void UALSDebugComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	// Keep static values false on destroy
	bDebugView = false;
	bShowTraces = false;
	bShowDebugShapes = false;
	bShowLayerColors = false;
}

void UALSDebugComponent::PreviousFocusedDebugCharacter()
{
	if (FocusedDebugCharacterIndex == INDEX_NONE)
	{ // Return here as no AALSBaseCharacter where found during call of BeginPlay.
		// Moreover, for savety set also no focused debug character.
		DebugFocusCharacter = nullptr;
		return;
	}

	FocusedDebugCharacterIndex++;
	if (FocusedDebugCharacterIndex >= AvailableDebugCharacters.Num()) {
		FocusedDebugCharacterIndex = 0;
	}

	DebugFocusCharacter = AvailableDebugCharacters[FocusedDebugCharacterIndex];
}

void UALSDebugComponent::NextFocusedDebugCharacter()
{
	if (FocusedDebugCharacterIndex == INDEX_NONE)
	{ // Return here as no AALSBaseCharacter where found during call of BeginPlay.
		// Moreover, for savety set also no focused debug character.
		DebugFocusCharacter = nullptr;
		return;
	}

	FocusedDebugCharacterIndex--;
	if (FocusedDebugCharacterIndex < 0) {
		FocusedDebugCharacterIndex = AvailableDebugCharacters.Num() - 1;
	}

	DebugFocusCharacter = AvailableDebugCharacters[FocusedDebugCharacterIndex];
}

void UALSDebugComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
	DebugFocusCharacter = OwnerCharacter;
	if (OwnerCharacter)
	{
		SetDynamicMaterials();
		SetResetColors();
	}

	// Get all ALSBaseCharacter's, which are currently present to show them later in the ALS HUD for debugging purposes.
	TArray<AActor*> AlsBaseCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AALSBaseCharacter::StaticClass(), AlsBaseCharacters);

	AvailableDebugCharacters.Empty();
	if (AlsBaseCharacters.Num() > 0)
	{
		AvailableDebugCharacters.Reserve(AlsBaseCharacters.Num());
		for (auto Character : AlsBaseCharacters)
		{
			if (auto AlsBaseCharacter = Cast<AALSBaseCharacter>(Character))
			{
				AvailableDebugCharacters.Add(AlsBaseCharacter);
			}
		}

		FocusedDebugCharacterIndex = AvailableDebugCharacters.Find(DebugFocusCharacter);
		if (FocusedDebugCharacterIndex == INDEX_NONE && AvailableDebugCharacters.Num() > 0)
		{ // seems to be that this component was not attached to and AALSBaseCharacter,
			// therefore the index will be set to the first element in the array.
			FocusedDebugCharacterIndex = 0;
		}
	}
}

void UALSDebugComponent::ToggleGlobalTimeDilationLocal(float TimeDilation)
{
	if (UKismetSystemLibrary::IsStandalone(this))
	{
		UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
	}
}

void UALSDebugComponent::ToggleSlomo()
{
	bSlomo = !bSlomo;
	ToggleGlobalTimeDilationLocal(bSlomo ? 0.15f : 1.f);
}

void UALSDebugComponent::ToggleDebugView()
{
	bDebugView = !bDebugView;

	AALSPlayerCameraManager* CamManager = Cast<AALSPlayerCameraManager>(
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CamManager)
	{
		UALSPlayerCameraBehavior* CameraBehavior = Cast<UALSPlayerCameraBehavior>(
			CamManager->CameraBehavior->GetAnimInstance());
		if (CameraBehavior)
		{
			CameraBehavior->bDebugView = bDebugView;
		}
	}
}

void UALSDebugComponent::ToggleDebugMesh()
{
	if (bDebugMeshVisible)
	{
		OwnerCharacter->SetVisibleMesh(DefaultSkeletalMesh);
	}
	else
	{
		DefaultSkeletalMesh = OwnerCharacter->GetMesh()->SkeletalMesh;
		OwnerCharacter->SetVisibleMesh(DebugSkeletalMesh);
	}
	bDebugMeshVisible = !bDebugMeshVisible;
}
