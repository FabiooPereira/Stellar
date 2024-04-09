// Fill out your copyright notice in the Description page of Project Settings.


#include "AlpacaMovement.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"

// Sets default values
AAlpacaMovement::AAlpacaMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCharacterptr = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
}

// Called when the game starts or when spawned
void AAlpacaMovement::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlpacaMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(IsFollowing)
	{
		PlayerCharacterptr = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
		FollowCharacter(PlayerCharacterptr, DeltaTime);
	}
	DrawDebugOffset(PlayerCharacterptr);
	
	
}

// Called to bind functionality to input
void AAlpacaMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("Stay"), IE_Pressed,this, &AAlpacaMovement::Stay);
}
//Moves the Companion(Self) to a targetLocation and stops when it reaches the 
void AAlpacaMovement::MoveToLocation(FVector& TargetLocation,const float DeltaTime)
{
	if(!TargetLocation.IsZero())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation,TargetLocation,DeltaTime,MovementSpeed);
		SetActorLocation((NewLocation));
		//May not be necesary because we already create a distance from the character to follow in the FollowCharacter method!!
		// if (FVector::DistSquared(CurrentLocation, TargetLocation) < FMath::Square(10.0f))
		// {
		// 	TargetLocation = FVector::ZeroVector;
		// }
	}
}
//Makes the Companion(self) follow a ACharacter.
void AAlpacaMovement::FollowCharacter(const ACharacter* CharacterToFollow,const float DeltaTime)
{
	if(CharacterToFollow)
	{
		FVector Offset = GetActorLocation() - PlayerCharacterptr->GetActorLocation();
		Offset.Normalize();
		FVector TargetLocation =PlayerCharacterptr->GetActorLocation() + Offset * FollowDistance;
		
		MoveToLocation(TargetLocation, DeltaTime);
	}
}
//Draws a sphere around the player that shows the limit(FollowDistance) for how close the companion can get to the player. 
void AAlpacaMovement::DrawDebugOffset(const ACharacter* Player)const
{
	if(Player)
	{
		DrawDebugSphere(GetWorld(), Player->GetActorLocation(),FollowDistance, 12, FColor::Green,false, 0.1);

	}
}
void AAlpacaMovement::Stay()
{
	IsFollowing = false;
	UE_LOG(LogTemp, Warning, TEXT("Stay!!"));
}
void AAlpacaMovement::Call()
{
	IsFollowing = true;
}





