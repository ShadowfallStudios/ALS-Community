// Project:         Advanced Locomotion System V4 on C++
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Haziq Fadhil
// Contributors:    Doga Can Yanikoglu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ALSCharacterMovementComponent.generated.h"

/**
 * Authoritative networked Character Movement
 */
UCLASS()
class ALSV4_CPP_API UALSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

	class FSavedMove_My : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
		                        class FNetworkPredictionData_Client_Character& ClientData) override;

		// Walk Speed Update
		uint8 bSavedRequestMovementSettingsChange : 1;
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
	virtual void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;


	// Movement Settings Variables
	uint8 bRequestMovementSettingsChange = 1;

	float MyNewMaxWalkSpeed = 0;

	// Set Max Walking Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "Movement Settings")
	void SetMaxWalkingSpeed(float NewMaxWalkSpeed);

	UFUNCTION(Reliable, Server)
	void Server_SetMaxWalkingSpeed(float NewMaxWalkSpeed);
};
