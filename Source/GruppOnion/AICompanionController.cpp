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

	CurrentState = EAICompanionState::Idle;
}


void AAICompanionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case  EAICompanionState::Idle:
			IdleState();
		break;
	case EAICompanionState::Wander:
			WanderState();
		break;
	case EAICompanionState::Follow:
			FollowPlayer();
		break;
	case EAICompanionState::WalkAroundPlayer:
			WalkAroundPlayer(); 
		break;
	default:
			
		break;
	}
	
}


void AAICompanionController::IdleState()
{
	StopMovement();
	InitialIdlePosition = GetPawn()->GetActorLocation();
	//StopMovement();
	FVector AlpacaLocation = GetPawn()->GetActorLocation();
	
	DrawDebugSphere(GetWorld(),AlpacaLocation , 300, 12, FColor::Green, false, -1);
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

	if(!GetWorldTimerManager().IsTimerActive(IdleTimeLimitHandle))
	{
		GetWorldTimerManager().SetTimer(IdleTimeLimitHandle, this, &AAICompanionController::StartWandering, IdleTimeLimit, false);
	}
	
	if(CurrentDistanceToPlayer<300)
	{
		SetState(EAICompanionState::Follow);
	}
	// GetWorldTimerManager().SetTimer(WanderingTimerHandle, this, &AAICompanionController::StartWandering, 3.0f, false
	// 	);
}
void AAICompanionController::StartWandering()
{
	SetState(EAICompanionState::Wander);
}
void AAICompanionController::WalkAroundPlayer(){
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Transitioning to Follow State"))
		SetState(EAICompanionState::Follow);
	}
}

void AAICompanionController::FollowPlayer()
{
	//Calculate the distance between AI companion and player
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

	// if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
	// {
		 FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
		 FVector TargetLocation = PlayerCharacter->GetActorLocation() - DirectionToPlayer * 150.0f; // Adjust the offset as needed
		 MoveToLocation(TargetLocation);
		 ////MoveToActor(PlayerCharacter, 150,true, true,true,0,true
		 	////);
		// Get the AI's velocity
		//FVector Velocity = GetPawn()->GetVelocity();

		// Calculate the rotation based on the velocity direction
		//FRotator NewRotation = Velocity.Rotation();
    
		// Apply the new rotation to the AI
		//GetPawn()->SetActorRotation(NewRotation);

		// Move the AI towards the player
		//MoveToActor(PlayerCharacter, 150,true, true,true,0,true);
	if(CurrentDistanceToPlayer<=200)
	{
		UE_LOG(LogTemp, Warning, TEXT("Transitioning to Idle state"));
		SetState(EAICompanionState::WalkAroundPlayer);
	}
	
}  

void AAICompanionController::StopFollowPlayer()
{
	bShouldFollowPlayer = false;
	SetState(EAICompanionState::Idle);
}

void AAICompanionController::ContinueFollowPlayer()
{
	bShouldFollowPlayer = true;
	SetState(EAICompanionState::Follow);
}
//-------------------------------------------------States-------------------------------------------------------------//

void AAICompanionController::SetState(EAICompanionState NewState)
{
	if(NewState == EAICompanionState::WalkAroundPlayer)
	{
		GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation, 2.0f, false);
	}
	else if(NewState == EAICompanionState::Follow)
	{
		GetWorldTimerManager().ClearTimer(IdleTimeLimitHandle);
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
	}
	else if(NewState == EAICompanionState::Idle)
	{
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
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
	
	if (CurrentState == EAICompanionState::WalkAroundPlayer)
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
void AAICompanionController::WanderState()
{
	FVector AlpacaLocation = GetPawn()->GetActorLocation();
	
	DrawDebugSphere(GetWorld(),AlpacaLocation , 300, 12, FColor::Green, false, -1);
	
	if(!GetWorldTimerManager().IsTimerActive(WanderingTimerHandle))
	{
		GetWorldTimerManager().SetTimer(WanderingTimerHandle, this, &AAICompanionController::WanderNewRandomLocation, 2, false);
	}
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if(CurrentDistanceToPlayer<300)
	{
		SetState(EAICompanionState::Follow);
	}
}
void AAICompanionController::WanderNewRandomLocation()
{
	// Calculate random location relative to the initial idle position
	FVector2D RandomLocation2D = FMath::RandPointInCircle(600);
	FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0f) + InitialIdlePosition;
	DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 12, FColor::Green, false, 5.0f);

	// Move to the random location
	MoveToLocation(RandomLocation);
}
