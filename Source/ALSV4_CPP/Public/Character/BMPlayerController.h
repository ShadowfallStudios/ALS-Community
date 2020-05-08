// Copyright (C) 2020, Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BMPlayerController.generated.h"

class ABMBaseCharacter;

/**
* Player controller class
*/
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API ABMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void RestartPawn(APawn* NewPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRestartPawn(APawn* NewPawn);

private:
	/** Main character reference */
	ABMBaseCharacter* PossessedCharacter = nullptr;
};
