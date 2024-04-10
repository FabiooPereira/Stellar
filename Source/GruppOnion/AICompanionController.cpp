// Fill out your copyright notice in the Description page of Project Settings.


#include "AICompanionController.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AAICompanionController::BeginPlay()
{
	Super::BeginPlay();

	// Find the player character
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AAICompanionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Follow the player if a valid player character is found
	if (PlayerCharacter && bShouldFollowPlayer)
	{
		if (GetPawn())
		{
			//UE_LOG(LogTemp, Error, TEXT("owner found"))
			double DistanceTo = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

			if (DistanceTo > FollowDistance)
			{
				FollowPlayer();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("no owner found"))
		}
	}
}


void AAICompanionController::FollowPlayer()
{
	// Calculate the target location to follow the player
	FVector TargetLocation = PlayerCharacter->GetActorLocation() - FVector(FollowDistance, 0.0f, 0.0f);
	// Adjust the offset as needed

	// Move the companion towards the target location
	MoveToLocation(TargetLocation);
}

void AAICompanionController::StopFollowPlayer()
{
	bShouldFollowPlayer = false;
	StopMovement();
}

void AAICompanionController::ContinueFollowPlayer()
{
	bShouldFollowPlayer = true;
	FollowPlayer();
}
