// Fill out your copyright notice in the Description page of Project Settings.


#include "AICompanionController.h"
#include "AIHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

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
	//HandleCollisionWithPlayer();
	//StateMachine
	switch (CurrentState)
	{
	case EAICompanionState::Idle:
		IdleState();
		break;
	case EAICompanionState::WanderAround:
		WanderState();
		break;
	case EAICompanionState::Startled:
		StartledState();
		break;
	case EAICompanionState::MoveToLocationAndIdleState:
		GoToCommandedTarget();
		break;
	case EAICompanionState::FocusState:
		FocusState();
		break;
	case EAICompanionState::MountedState:
	default:

		break;
	}
}

void AAICompanionController::HandleCollisionWithPlayer()
{
	
		// Get the collision capsule component of the player character
		UCapsuleComponent* PlayerCapsule = PlayerCharacter->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			// Get the collision capsule component of the companion character
			ACharacter* CompanionCharacter = Cast<ACharacter>(GetPawn());
			UCapsuleComponent* CompanionCapsule = CompanionCharacter ? CompanionCharacter->GetCapsuleComponent() : nullptr;
			if (CompanionCapsule)
			{
				// Check if the companion character is overlapping with the player character's capsule
				bool bIsOverlapping = CompanionCapsule->IsOverlappingComponent(PlayerCapsule);
				if (bIsOverlapping)
				{
					UE_LOG(LogTemp,Warning,TEXT("Overlap"));
					// Calculate direction from companion to player
					FVector DirectionToPlayer =  CompanionCharacter->GetActorLocation()-PlayerCharacter->GetActorLocation();
					DirectionToPlayer.Z = 0.f; // Ignore vertical component
					DirectionToPlayer.Normalize();

					// Calculate new position for companion (move aside)
					FVector NewPosition = CompanionCharacter->GetActorLocation() + DirectionToPlayer * 200.0f;

					// Move companion to new position
					//CompanionCharacter->SetActorLocation(NewPosition);
					MoveToLocation(NewPosition);
				}
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("no capsule found"));
			}
		}
	
}

//-----------------------------------------Methods--------------------------------------------//

//HelpMethod to change state
void AAICompanionController::StartWandering()
{
	SetState(EAICompanionState::WanderAround);
}

//---------------------------------------Player Calls-----------------------------------------//

void AAICompanionController::CallToLocation(FVector targetLocation)
{

		UE_LOG(LogTemp,Warning,TEXT("Call to Location"))
		IsGoingToCommandedTarget = false;
		TargetToStandOn = targetLocation;

		SetState(EAICompanionState::MoveToLocationAndIdleState);
}


void AAICompanionController::GoToCommandedTarget()
{
	if(IsGoingToCommandedTarget)
	{
		if (FVector::Dist(GetPawn()->GetActorLocation(), TargetToStandOn)<200.f)
		{
			
			IsGoingToCommandedTarget=false;
			if(!GetWorldTimerManager().IsTimerActive(SetStateHandler))
			{
				//UE_LOG(LogTemp,Warning,TEXT("Transition to Idle timer has been set"));
				//GetWorldTimerManager().SetTimer(SetStateHandler, this,&AAICompanionController::StartIdle, 10,false);
			}
			//CheckIfShouldFocusPlayer();
		}
		return;
	}
	MoveToLocation(TargetToStandOn);
	IsGoingToCommandedTarget = true;
	
}

void AAICompanionController::FocusState()
{
	if(FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation())>300.0f)
	{
		SetState(EAICompanionState::Idle);
	}
	else
	{
		FVector PlayerDirection = PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation();
		PlayerDirection.Z = 0.f;
		PlayerDirection.Normalize();

		FRotator NewRotation = PlayerDirection.Rotation();
		GetPawn()->SetActorRotation(NewRotation);
		
	}
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
		UE_LOG(LogTemp, Warning, TEXT("Transition into WanderAround"));
		break;
	
	case EAICompanionState::Startled:
		UE_LOG(LogTemp, Warning, TEXT("Transition into Startled"))
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
		break;
	case EAICompanionState::MoveToLocationAndIdleState:
		GetWorldTimerManager().ClearTimer(SetStateHandler);
		GetWorldTimerManager().ClearTimer(RandomMoveTimerHandle);
		GetWorldTimerManager().ClearTimer(WanderingTimerHandle);
		GetWorldTimerManager().ClearTimer(StartledTimerHandle);
		GetWorldTimerManager().ClearTimer(IdleTimeLimitHandle);
		break;
	case EAICompanionState::FocusState:
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
	//DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 300, 12, FColor::Green, false, -1);
	
	//Sets a timer to change current state within a certain time.
	if (!GetWorldTimerManager().IsTimerActive(IdleTimeLimitHandle))
	{
		UE_LOG(LogTemp,Warning, TEXT("IdleToWanderTransition is set"));
		GetWorldTimerManager().SetTimer(IdleTimeLimitHandle, this, &AAICompanionController::StartWandering,
		                                5, false);
	}
	CheckIfShouldFocusPlayer();
}

void AAICompanionController::WanderState()
{
	
	//Draws a sphere that shows how close the player should be to get the companions Attention.
	//DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 300, 12, FColor::Green, false, -1);
	
	//Sets a timer so the companion can go to different random locations
	if (!GetWorldTimerManager().IsTimerActive(WanderingTimerHandle))
	{
		GetWorldTimerManager().SetTimer(WanderingTimerHandle, this, &AAICompanionController::WanderNewRandomLocation,
		                                FMath::RandRange(5, 8), false);
	}
	CheckIfShouldFocusPlayer();
}

void AAICompanionController::WanderNewRandomLocation()
{
	// Calculate random location relative to the initial idle position
	FVector2D RandomLocation2D = FMath::RandPointInCircle(500);
	FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0f) + InitialIdlePosition;

	//Draws sphere to the next targetLocation
	//DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 12, FColor::Green, false, 5.0f);

	// Move to the random location
	MoveToLocation(RandomLocation);
}


void AAICompanionController::CheckIfShouldFocusPlayer()
{
	float CurrentDistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation());
	if (CurrentDistanceToPlayer < DistanceToGetCompanionFocus)
	{
		MoveToActor(PlayerCharacter, 100, true,true,true
			,0,true);
		SetState(EAICompanionState::FocusState);
		
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

		FVector TargetLocation = GetPawn()->GetActorLocation() + DirectionAwayFromDarkness * 300;
		//DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, FColor::Green, false, 5.0f);

		MoveToLocation(TargetLocation);
	}
	float DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), DarknessActorRef->GetActorLocation());
	if(!GetWorldTimerManager().IsTimerActive(StartledTimerHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle timer has been set"))
		GetWorldTimerManager().SetTimer(StartledTimerHandle, this, &AAICompanionController::StartIdle, MaxStartledTime, false);
	}
	
	if (DistanceToTarget >= 300)
	{
		SetState(EAICompanionState::Idle);
		IsRunningAway = false;
	}
}


void AAICompanionController::StartIdle()
{
	IsRunningAway = false;
	SetState(EAICompanionState::Idle);
}


void AAICompanionController::CheckForDarknessOverlap()
{
	TArray<AActor*> OverlappingActors;
	GetPawn()->GetOverlappingActors(OverlappingActors);


	for (AActor* OverlappingActor : OverlappingActors)
	{
		for(auto c:OverlappingActor->GetComponents())
		{
			if(c->ComponentHasTag("Darkness"))
			{
				UE_LOG(LogTemp, Warning, TEXT("Alpaca Is Inside Darkness"))
				DarknessActorRef = OverlappingActor;
				SetState(EAICompanionState::Startled);
			}
		}
		//
		// if (OverlappingActor->ActorHasTag("Darkness") && CurrentState != EAICompanionState::Startled)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Alpaca Is Inside Darkness"))
		// 	DarknessActorRef = OverlappingActor;
		// 	SetState(EAICompanionState::Startled);
		// }
	}
}
