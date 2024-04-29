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
	WanderAround,
	FollowPlayer,
	WanderAroundPlayer,
	Startled
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

	FTimerHandle IdleTimeLimitHandle;
	float IdleTimeLimit = 3.0;
	FTimerHandle RandomMoveTimerHandle;
	FTimerHandle WanderingTimerHandle;

	AActor* DarknessActorRef;


	FVector InitialIdlePosition;

protected:
	virtual void BeginPlay() override;

	EAICompanionState CurrentState;

public:
	virtual void Tick(float DeltaTime) override;

	//Functions Player can call for the companion to execute
	void StopFollowPlayer();
	void ContinueFollowPlayer();

	//-------------------------------SetStates----------------------------------//
	void SetState(EAICompanionState NewState);
	//---------------------------------States-----------------------------------//
	void IdleState();
	void WanderState();
	void FollowPlayerState();
	void WanderAroundPlayerState();
	void StartledState();
	//-------------------------------HelpMethods-------------------------------//
	void StartWandering();
	void SetRandomLocationTimer();
	void ChooseNewRandomLocation();
	void WanderNewRandomLocation();
	void CheckForDarknessOverlap();
	
	bool IsRunningAway = false;

private:
	float MaxDistanceAllowedFromPlayer = 600.0f;

	bool bShouldFollowPlayer = true;
};
