// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController.h"
#include "AICompanionController.generated.h"

//Define enum for AI companion states
UENUM(BlueprintType)
enum class EAICompanionState : uint8
{
	Idle,
	WanderAround,
	FollowPlayer,
	WanderAroundPlayer,
	Startled,
	MoveToLocationAndIdleState
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
	float DistanceToGetCompanionFocus = 300.0f;
	FTimerHandle IdleTimeLimitHandle;
	float IdleTimeLimit = 3.0;
	FTimerHandle RandomMoveTimerHandle;
	FTimerHandle WanderingTimerHandle;
	FTimerHandle StartledTimerHandle;
	FTimerHandle WanderingAroundPlayerTimerHandle;
	FTimerHandle SetStateHandler;

	
	float MaxStartledTime = 2.0;

	AActor* DarknessActorRef;


	FVector InitialIdlePosition;

	FVector TargetToStandOn;
	bool IsGoingToCommandedTarget;

protected:
	virtual void BeginPlay() override;

	EAICompanionState CurrentState;

public:
	virtual void Tick(float DeltaTime) override;

	//Functions Player can call for the companion to execute
	void StopFollowPlayer();
	void ContinueFollowPlayer();
	UFUNCTION(BlueprintCallable)
	void SendToLocation(FVector targetLocation, bool IsButton);

	//-------------------------------SetStates----------------------------------//
	UFUNCTION(BlueprintCallable)
	void SetState(EAICompanionState NewState);
	//---------------------------------States-----------------------------------//
	void IdleState();
	void WanderState();
	void FollowPlayerState();
	void WanderAroundPlayerState();
	void StartledState();
	void GoToCommandedTarget();
	//-------------------------------HelpMethods-------------------------------//
	void StartWanderAroundPlayer();
	void StartWandering();
	void StartIdle();
	//void SetRandomLocationTimer();
	void ChooseNewRandomLocation();
	void WanderNewRandomLocation();
	void CheckForDarknessOverlap();
	
	bool IsRunningAway = false;
	UPROPERTY(BlueprintReadOnly)
	bool CallStayToogle = false;

private:
	void CheckIfShouldFocusPlayer();

	
	float MaxDistanceAllowedFromPlayer = 600.0f;

	bool bShouldFollowPlayer = true;
};
