// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:	Drakynfly

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ALSPlayerController.generated.h"

class AALSBaseCharacter;

/**
 * Player controller class
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

private:
	void SetupCamera();

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleDebugView();

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "ALS Player Controller")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "ALS Player Controller")
	AALSBaseCharacter* PossessedCharacter = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bShowHud = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bDebugView = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bShowTraces = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bShowDebugShapes = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bShowLayerColors = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS Player Controller")
	bool bShowCharacterInfo = false;
};
