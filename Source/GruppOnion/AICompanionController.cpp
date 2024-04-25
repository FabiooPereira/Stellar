// Fill out your copyright notice in the Description page of Project Settings.


#include "AICompanionController.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void ChooseNewRandomLocation();
void AAICompanionController::BeginPlay()
{
	Super::BeginPlay();

	// Find the player character
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	CurrentState = EAICompanionState::Follow;
}

void AAICompanionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case  EAICompanionState::Idle:
			IdleState();
		break;
	case EAICompanionState::Follow:
			FollowPlayer();
		break;
	default:
			
		break;
	}
	
}

void AAICompanionController::FollowPlayer()
{
	//Calculate the distance between AI companion and player
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

	if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
	{
		FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
		FVector TargetLocation = PlayerCharacter->GetActorLocation() - DirectionToPlayer * 100.0f; // Adjust the offset as needed
		MoveToLocation(TargetLocation);
		
	}else if(CurrentDistanceToPlayer<=200)
	{
		UE_LOG(LogTemp, Warning, TEXT("Transitioning to Idle state"));
		SetState(EAICompanionState::Idle);
	}
	
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

void AAICompanionController::IdleState()
{
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Transitioning to Follow State"))
		SetState(EAICompanionState::Follow);
	}
	
}

void AAICompanionController::SetState(EAICompanionState NewState)
{
	if(NewState == EAICompanionState::Idle)
	{
		GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation, 2.0f, false);
	}
	else if(NewState == EAICompanionState::Follow)
	{
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
	}
	CurrentState = NewState;
}

void AAICompanionController::SetRandomLocationTimer()
{
	float RandomDelay = FMath::RandRange(2.0f, 5.0f);
	GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation, RandomDelay, false);

}

void AAICompanionController::ChooseNewRandomLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("ChooseNewRandomLocation called"))
	
	if (CurrentState == EAICompanionState::Idle)
	{
		float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
		if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Transitioning to Follow State"))
			SetState(EAICompanionState::Follow);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Choosing new random location"));
		// Calculate random location within a radius from the player
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector2D RandomLocation2D = FMath::RandPointInCircle((MaxDistanceAllowedFromPlayer/2)-10);
		FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0f) + PlayerLocation;
			if(FVector::Dist(RandomLocation,GetPawn()->GetActorLocation())<300)
		{
			RandomLocation = PlayerLocation + (RandomLocation -PlayerLocation).GetSafeNormal() * 300; 
		}
		DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 12, FColor::Green, false, 5.0f);

		// Move to the random location
		MoveToLocation(RandomLocation);
			SetRandomLocationTimer();
		}
	}
}
