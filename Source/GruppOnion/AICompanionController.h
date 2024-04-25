// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController.h"
#include "AICompanionController.generated.h"

//Define enum for AI companion states
enum class EAICompanionState : uint8
{
	Idle,
	Follow,
	WonderAround,
	Stay
};
/**
 * 
 */
UCLASS()
class GRUPPONION_API AAICompanionController : public AAIController
{
	GENERATED_BODY()
public:
	ACharacter* PlayerCharacter;
	float FollowDistance = 400.0f; // Limit for how close it can get to the PlayerCharacter
	FTimerHandle RandomMoveTimerHandle;
protected:
	virtual void BeginPlay() override;

	EAICompanionState CurrentState;
public:
	virtual void Tick(float DeltaTime) override;

	// Function to make the companion follow the player
	void FollowPlayer();
	//Funtion to make the  companion to stop following the player
	void StopFollowPlayer();
	//Function to call for the Companion
	void ContinueFollowPlayer();
	//IdleState
	void IdleState();
	//SetState
	void SetState(EAICompanionState NewState);
	//SetTimerFornew location
	void SetRandomLocationTimer();

	void ChooseNewRandomLocation();
private:
	float MaxDistanceAllowedFromPlayer = 600.0f;
	
	bool bShouldFollowPlayer= true;
};