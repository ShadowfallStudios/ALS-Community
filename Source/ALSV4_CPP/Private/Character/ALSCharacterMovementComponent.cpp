// Project:         Advanced Locomotion System V4 on C++
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Haziq Fadhil
// Contributors:    


#include "Character/ALSCharacterMovementComponent.h"
#include "Character/ALSBaseCharacter.h"

UALSCharacterMovementComponent::UALSCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UALSCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	// Set Movement Settings
	if (bRequestMovementSettingsChange)
	{
		MaxWalkSpeed = MyNewMaxWalkSpeed;
		MaxWalkSpeedCrouched = MyNewMaxWalkSpeed;
		MaxAcceleration = MyNewMaxAcceleration;
		BrakingDecelerationWalking = MyNewBraking;
		GroundFriction = MyNewGroundFriction;

		// Ensures server Movement Settings values updates to latest
		bRequestMovementSettingsChange = MaxAcceleration != RealMaxAcceleration
			|| BrakingDecelerationWalking != RealBraking
			|| GroundFriction != RealGroundFriction;
	}
}

void UALSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags) // Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMovementSettingsChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UALSCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UALSCharacterMovementComponent* MutableThis = const_cast<UALSCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UALSCharacterMovementComponent::FSavedMove_My::Clear()
{
	Super::Clear();

	bSavedRequestMovementSettingsChange = false;
}

uint8 UALSCharacterMovementComponent::FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMovementSettingsChange)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool UALSCharacterMovementComponent::FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character,
                                                                   float MaxDelta) const
{
	// Set which moves can be combined together. This will depend on the bit flags that are used.	
	if (bSavedRequestMovementSettingsChange != ((FSavedMove_My*)&NewMove)->bSavedRequestMovementSettingsChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UALSCharacterMovementComponent::FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
                                                               class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UALSCharacterMovementComponent* CharacterMovement = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMovementSettingsChange = CharacterMovement->bRequestMovementSettingsChange;
	}
}

void UALSCharacterMovementComponent::FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UALSCharacterMovementComponent* CharacterMovement = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
}

UALSCharacterMovementComponent::FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(
	const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UALSCharacterMovementComponent::FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return MakeShared<FSavedMove_My>();
}

// Set Movement Settings RPC to transfer the current Movement Settings from the Owning Client to the Server
bool UALSCharacterMovementComponent::Server_SetMaxWalkingSpeed_Validate(const float NewMaxWalkSpeed)
{
	if (NewMaxWalkSpeed < 0.f || NewMaxWalkSpeed > 2000.f)
		return false;
	return true;
}

void UALSCharacterMovementComponent::Server_SetMaxWalkingSpeed_Implementation(const float NewMaxWalkSpeed)
{
	MyNewMaxWalkSpeed = NewMaxWalkSpeed;
}

void UALSCharacterMovementComponent::SetMaxWalkingSpeed(float NewMaxWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewMaxWalkSpeed = NewMaxWalkSpeed;
		Server_SetMaxWalkingSpeed(NewMaxWalkSpeed);
	}
	bRequestMovementSettingsChange = true;
}

// Set Max Walking Speed RPC to transfer the current Max Walking Speed from the Owning Client to the Server
bool UALSCharacterMovementComponent::Server_SetMovementSettings_Validate(const FVector NewMovementSettings)
{
	return true;
}

void UALSCharacterMovementComponent::Server_SetMovementSettings_Implementation(const FVector NewMovementSettings)
{
	MyNewMaxAcceleration = NewMovementSettings.X;
	MyNewBraking = NewMovementSettings.Y;
	MyNewGroundFriction = NewMovementSettings.Z;
	bRequestMovementSettingsChange = true;
}

void UALSCharacterMovementComponent::SetMovementSettings(FVector NewMovementSettings)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewMaxAcceleration = NewMovementSettings.X;
		MyNewBraking = NewMovementSettings.Y;
		MyNewGroundFriction = NewMovementSettings.Z;
		Server_SetMovementSettings(NewMovementSettings);
	}
	bRequestMovementSettingsChange = true;

	// Save Server Movement Settings for comparison during movement update
	RealMaxAcceleration = NewMovementSettings.X;
	RealBraking = NewMovementSettings.Y;
	RealGroundFriction = NewMovementSettings.Z;
}
