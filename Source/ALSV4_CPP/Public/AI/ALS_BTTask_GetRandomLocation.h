// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ALS_BTTask_GetRandomLocation.generated.h"

class UNavigationQueryFilter;

/** Picks a random location reachable through NavMesh within the Max Distance from the Owning Pawn's current location and assigns it to the specified Blackboard Key. */
UCLASS(Category=ALS, meta=(DisplayName = "Get Random Location"))
class ALSV4_CPP_API UALS_BTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UALS_BTTask_GetRandomLocation();

	/** Maximum distance the random location picked may be from pawn. */
	UPROPERTY(Category = Navigation, EditAnywhere, meta=(ClampMin = 1))
	float MaxDistance = 1000.0f;

	/** Navigation Query filter used when picking the location, only picks locations reachable using this filter when assigned. */
	UPROPERTY(Category = Navigation, EditAnywhere)
	TSubclassOf<UNavigationQueryFilter> Filter = nullptr;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
