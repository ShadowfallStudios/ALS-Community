// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu
// Contributors:    

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ALSDebugComponent.generated.h"

class AALSCharacter;

UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UALSDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UALSDebugComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/** Implemented on BP to update layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS|Debug")
	void UpdateColoringSystem();

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS|Debug")
	void DrawDebugSpheres();

	/** Implemented on BP to set/reset layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS|Debug")
	void SetResetColors();

	/** Implemented on BP to set dynamic color materials for debugging */
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS|Debug")
	void SetDynamicMaterials();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleDebugView();

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
    bool GetShowTraces() { return bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
    bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "ALS|Debug")
    bool GetShowLayerColors() { return bShowLayerColors; }

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Debug")
	AALSCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bShowHud = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALS|Debug")
	bool bShowCharacterInfo = false;

	static bool bDebugView;

	static bool bShowTraces;

	static bool bShowDebugShapes;

	static bool bShowLayerColors;

private:
	bool bNeedsColorReset = false;
};
