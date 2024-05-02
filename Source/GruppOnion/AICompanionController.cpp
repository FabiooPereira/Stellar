// Fill out your copyright notice in the Description page of Project Settings.


#include "AICompanionController.h"

#include "AIHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void ChooseNewRandomLocation();

void AAICompanionController::BeginPlay()
{
	Super::BeginPlay();

	// Find the player character
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//InitialSTate set to IDLE
	CurrentState = EAICompanionState::Idle;
}


void AAICompanionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Checks if Pawn is overlaping with Enemy
	CheckForDarknessOverlap();
	//StateMachine
	switch (CurrentState)
	{
	case EAICompanionState::Idle:
		IdleState();
		break;
	case EAICompanionState::WanderAround:
		WanderState();
		break;
	case EAICompanionState::FollowPlayer:
		FollowPlayerState();
		break;
	case EAICompanionState::WanderAroundPlayer:
		WanderAroundPlayerState();
		break;
	case EAICompanionState::Startled:
		StartledState();
		break;
	default:

		break;
	}
}

//-----------------------------------------Methods--------------------------------------------//

//HelpMethod to change state
void AAICompanionController::StartWandering()
{
	SetState(EAICompanionState::WanderAround);
}

//---------------------------------------Player Calls-----------------------------------------//
void AAICompanionController::StopFollowPlayer()
{
	bShouldFollowPlayer = false;
	SetState(EAICompanionState::Idle);
}

void AAICompanionController::ContinueFollowPlayer()
{
	bShouldFollowPlayer = true;
	SetState(EAICompanionState::FollowPlayer);
}

//-------------------------------------------------States-------------------------------------------------------------//
void AAICompanionController::SetState(EAICompanionState NewState)
{
	
	switch (NewState)
	{
	case EAICompanionState::Idle:
		UE_LOG(LogTemp, Warning, TEXT("Transition into Idle"))
		IsRunningAway = false;
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
		GetWorldTimerManager().ClearTimer(StartledTimerHandle);
		break;
	case EAICompanionState::WanderAround:
		UE_LOG(LogTemp, Warning, TEXT("Transition into WanderAround"))
		GetWorldTimerManager().ClearTimer(WanderingAroundPlayerTimerHandle);
		break;
	case EAICompanionState::FollowPlayer:
		UE_LOG(LogTemp, Warning, TEXT("Transition into FollowState"))
		GetWorldTimerManager().ClearTimer(IdleTimeLimitHandle);
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
		break;
	case EAICompanionState::WanderAroundPlayer:
		UE_LOG(LogTemp, Warning, TEXT("Transition into WanderAroundPlayerState"))
		//Potentialbug
		// GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation,
		// 								3.0f, false);
		//GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		break;
	case EAICompanionState::Startled:
		UE_LOG(LogTemp, Warning, TEXT("Transition into Startled"))
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
		break;
	}
	CurrentState = NewState;
}

void AAICompanionController::IdleState()
{
	IsRunningAway = false;
	InitialIdlePosition = GetPawn()->GetActorLocation();
	//Draws a sphere that shows how close the player should be to get the companions Attention.
	DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 300, 12, FColor::Green, false, -1);
	
	//Sets a timer to change current state within a certain time.
	if (!GetWorldTimerManager().IsTimerActive(IdleTimeLimitHandle))
	{
		UE_LOG(LogTemp,Warning, TEXT("IdleToWanderTransition is set"));
		GetWorldTimerManager().SetTimer(IdleTimeLimitHandle, this, &AAICompanionController::StartWandering,
		                                IdleTimeLimit, false);
	}
	CheckIfShouldFocusPlayer();
}

void AAICompanionController::WanderState()
{
	
	//Draws a sphere that shows how close the player should be to get the companions Attention.
	DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 300, 12, FColor::Green, false, -1);
	CheckIfShouldFocusPlayer();
	//Sets a timer so the companion can go to different random locations
	if (!GetWorldTimerManager().IsTimerActive(WanderingTimerHandle))
	{
		GetWorldTimerManager().SetTimer(WanderingTimerHandle, this, &AAICompanionController::WanderNewRandomLocation,
		                                FMath::RandRange(2.0f, 5.0f), false);
	}
}

void AAICompanionController::WanderNewRandomLocation()
{
	// Calculate random location relative to the initial idle position
	FVector2D RandomLocation2D = FMath::RandPointInCircle(600);
	FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0f) + InitialIdlePosition;

	//Draws sphere to the next targetLocation
	DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 12, FColor::Green, false, 5.0f);

	// Move to the random location
	MoveToLocation(RandomLocation);
}

void AAICompanionController::FollowPlayerState()
{
	//Calculate the distance between AI companion and player
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

	// if(CurrentDistanceToPlayer>MaxDistanceAllowedFromPlayer)
	// {
	////Calculates next targetLocation
	
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
	if(CurrentDistanceToPlayer>300)
	{
		FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
		FVector TargetLocation = PlayerCharacter->GetActorLocation() - DirectionToPlayer * 150.0f;
		// Adjust the offset as needed
		MoveToLocation(TargetLocation);
		
		if(GetWorldTimerManager().IsTimerActive(WanderingAroundPlayerTimerHandle))
		{
			UE_LOG(LogTemp, Warning, TEXT("Wandering timer handle has been cleared"));
		GetWorldTimerManager().ClearTimer(WanderingAroundPlayerTimerHandle);
		}
	}
	if (CurrentDistanceToPlayer <= 200)
	{
		//SetState(EAICompanionState::WanderAroundPlayer);
		
		if(!GetWorldTimerManager().IsTimerActive(WanderingAroundPlayerTimerHandle))
		{
			UE_LOG(LogTemp, Warning, TEXT("Wandering timer handle has been set"));
		GetWorldTimerManager().SetTimer(WanderingAroundPlayerTimerHandle,this,&AAICompanionController::StartWanderAroundPlayer,3,false);
		}
	}
}

void AAICompanionController::WanderAroundPlayerState()
{
	//Draws a sphere around player to represent the limit for how far the companion can be from the player.
	DrawDebugSphere(GetWorld(), PlayerCharacter->GetActorLocation(), MaxDistanceAllowedFromPlayer, 12, FColor::Green, false, -1);

	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if (CurrentDistanceToPlayer > MaxDistanceAllowedFromPlayer)
	{
		SetState(EAICompanionState::FollowPlayer);
	}
	if (!GetWorldTimerManager().IsTimerActive(RandomMoveTimerHandle))
	{
		GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation,
		                                FMath::RandRange(2.0f, 5.0f), false);
	}
}

// void AAICompanionController::SetRandomLocationTimer()
// {
// 	float RandomDelay = FMath::RandRange(2.0f, 5.0f);
// 	GetWorldTimerManager().SetTimer(RandomMoveTimerHandle, this, &AAICompanionController::ChooseNewRandomLocation,
// 	                                RandomDelay, false);
// }

void AAICompanionController::ChooseNewRandomLocation()
{
	UE_LOG(LogTemp, Warning, TEXT("ChooseNewRandomLocation called"));

	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());

	if (CurrentDistanceToPlayer > MaxDistanceAllowedFromPlayer)
	{
		SetState(EAICompanionState::FollowPlayer);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Choosing new random location"));
		// Calculate random location within a radius from the player
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector2D RandomLocation2D = FMath::RandPointInCircle((MaxDistanceAllowedFromPlayer / 2) - 10);
		FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0f) + PlayerLocation;
		if (FVector::Dist(RandomLocation, GetPawn()->GetActorLocation()) < 300)
		{
			RandomLocation = PlayerLocation + (RandomLocation - PlayerLocation).GetSafeNormal() * 300;
		}

		//Draws sphere to next targetLocation
		DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 12, FColor::Green, false, 5.0f);

		// Move to the random location
		MoveToLocation(RandomLocation);
	}
}

void AAICompanionController::CheckIfShouldFocusPlayer()
{
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if (CurrentDistanceToPlayer < DistanceToGetCompanionFocus)
	{
		GetWorldTimerManager().ClearTimer(IdleTimeLimitHandle);
		SetState(EAICompanionState::FollowPlayer);
	}
}


void AAICompanionController::StartledState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Alpaca Is startled"))

	if (DarknessActorRef && !IsRunningAway)
	{
		IsRunningAway = true;

		FVector DirectionAwayFromDarkness = GetPawn()->GetActorLocation() - DarknessActorRef->GetActorLocation();
		DirectionAwayFromDarkness.Normalize();

		FVector TargetLocation = GetPawn()->GetActorLocation() + DirectionAwayFromDarkness * 1000;
		DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, FColor::Green, false, 5.0f);

		MoveToLocation(TargetLocation);
	}
	float DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), DarknessActorRef->GetActorLocation());
	if(!GetWorldTimerManager().IsTimerActive(StartledTimerHandle))
	{
		GetWorldTimerManager().SetTimer(StartledTimerHandle, this, &AAICompanionController::StartIdle, MaxStartledTime, false);
	}
	
	if (DistanceToTarget >= 1000)
	{
		SetState(EAICompanionState::Idle);
		IsRunningAway = false;
	}
}

void AAICompanionController::StartWanderAroundPlayer()
{
	SetState(EAICompanionState::WanderAroundPlayer);
}

void AAICompanionController::StartIdle()
{
	IsRunningAway = false;
	UE_LOG(LogTemp, Warning, TEXT("Idle timer has been set"))
	SetState(EAICompanionState::Idle);
}


void AAICompanionController::CheckForDarknessOverlap()
{
	TArray<AActor*> OverlappingActors;
	GetPawn()->GetOverlappingActors(OverlappingActors);


	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag("Darkness") && CurrentState != EAICompanionState::Startled)
		{
			UE_LOG(LogTemp, Warning, TEXT("Alpaca Is Inside Darkness"))
			DarknessActorRef = OverlappingActor;
			SetState(EAICompanionState::Startled);
		}
	}
}
