// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController.h"
#include "AICompanionController.generated.h"

/**
 * 
 */
UCLASS()
class GRUPPONION_API AAICompanionController : public AAIController
{
	GENERATED_BODY()
public:
	ACharacter* PlayerCharacter;
	float FollowDistance = 200.0f; // Limit for how close it can get to the PlayerCharacter
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Function to make the companion follow the player
	void FollowPlayer();
	//Funtion to make the  companion to stop following the player
	void StopFollowPlayer();
	//Function to call for the Companion
	void ContinueFollowPlayer();
private:
	bool bShouldFollowPlayer= true;
};
