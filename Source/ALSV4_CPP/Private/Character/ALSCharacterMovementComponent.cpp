// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ALSCharacterMovementComponent.h"
#include "Character/ALSBaseCharacter.h"

UALSCharacterMovementComponent::UALSCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UALSCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	//Set Max Walk Speed
	if (bRequestMaxWalkSpeedChange)
	{
		MaxWalkSpeed = MyNewMaxWalkSpeed;
		MaxWalkSpeedCrouched = MyNewMaxWalkSpeed;
		MaxAcceleration = MyNewMaxAcceleration;
		BrakingDecelerationWalking = MyNewBraking;
		GroundFriction = MyNewGroundFriction;
		bRequestMaxWalkSpeedChange = MaxAcceleration != MyNewMaxAcceleration;
	}
}

void UALSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMaxWalkSpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
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

	bSavedRequestMaxWalkSpeedChange = false;
}

uint8 UALSCharacterMovementComponent::FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool UALSCharacterMovementComponent::FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.	
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_My*)&NewMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UALSCharacterMovementComponent::FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UALSCharacterMovementComponent* CharacterMovement = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
	}
}

void UALSCharacterMovementComponent::FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UALSCharacterMovementComponent* CharacterMovement = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
}

UALSCharacterMovementComponent::FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UALSCharacterMovementComponent::FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}

//Set Max Walk Speed RPC to transfer the current Max Walk Speed from the Owning Client to the Server
bool UALSCharacterMovementComponent::Server_SetMaxWalkSpeedAndMaxAcceleration_Validate(const float NewMaxWalkSpeed, const float NewMaxAcceleration)
{
	if (NewMaxWalkSpeed < 0.f || NewMaxWalkSpeed > 2000.f)
		return false;
	else
		return true;
}

void UALSCharacterMovementComponent::Server_SetMaxWalkSpeedAndMaxAcceleration_Implementation(const float NewMaxWalkSpeed, const float NewMaxAcceleration)
{
	MyNewMaxWalkSpeed = NewMaxWalkSpeed;
	MyNewMaxAcceleration = NewMaxAcceleration;
}

void UALSCharacterMovementComponent::SetMaxWalkSpeedAndMaxAcceleration(float NewMaxWalkSpeed, float NewMaxAcceleration)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewMaxWalkSpeed = NewMaxWalkSpeed;
		MyNewMaxAcceleration = NewMaxAcceleration;
		Server_SetMaxWalkSpeedAndMaxAcceleration(NewMaxWalkSpeed, NewMaxAcceleration);
	}

	bRequestMaxWalkSpeedChange = true;
}

void UALSCharacterMovementComponent::SetBrakingAndGroundFriction(float NewBraking, float NewGroundFriction)
{
	MyNewBraking = NewBraking;
	MyNewGroundFriction = NewGroundFriction;

	bRequestMaxWalkSpeedChange = true;
}