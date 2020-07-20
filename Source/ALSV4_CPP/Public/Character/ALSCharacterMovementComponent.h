// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ALSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UALSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

		class FSavedMove_My : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(class ACharacter* Character) override;

		//Walk Speed Update
		uint8 bSavedRequestMaxWalkSpeedChange : 1;
	};

	class FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity);


	//Set Max Walk Speed
	uint8 bRequestMaxWalkSpeedChange : 1;

	float MyNewBraking;
	float MyNewGroundFriction;
	float MyNewMaxWalkSpeed;
	float MyNewMaxAcceleration;

	//Set Max Walk Speed (Called from the owning client)

	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetBrakingAndGroundFriction(float NewBraking, float NewGroundFriction);

	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetMaxWalkSpeedAndMaxAcceleration(float NewMaxWalkSpeed, float NewMaxAcceleration);

	UFUNCTION(reliable, Server, WithValidation)
	void Server_SetMaxWalkSpeedAndMaxAcceleration(const float NewMaxWalkSpeed, const float NewMaxAcceleration);
};

