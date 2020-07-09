// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/ALS_BTTask_GetRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UALS_BTTask_GetRandomLocation::UALS_BTTask_GetRandomLocation()
{
	NodeName = "Get Random Location";

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UALS_BTTask_GetRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UALS_BTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		if (AAIController* Controller = OwnerComp.GetAIOwner())
		{
			if(APawn* Pawn = Controller->GetPawn())
			{
				if (UWorld* World = GetWorld())
				{
					if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
					{
						FSharedConstNavQueryFilter sharedFilter = 0;

						if (Filter)
						{
							if (const ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate))
								sharedFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, World, Filter);
						}

						const FVector Origin = Pawn->GetActorLocation();
						FNavLocation Destination;

						if (NavSys->GetRandomReachablePointInRadius(Origin, MaxDistance, Destination, nullptr, sharedFilter))
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Destination.Location);
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

FString UALS_BTTask_GetRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Get Random Location\nMax Distance: %d\nFilter:%s"), FMath::RoundToInt(MaxDistance), Filter ? *GetNameSafe(Filter.Get()) : TEXT("None"));
}
