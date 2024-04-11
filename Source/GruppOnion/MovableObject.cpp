// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObject.h"

// Sets default values
AMovableObject::AMovableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovableObject::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("I EXIST LOL"))
}

// Called every frame
void AMovableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ShouldMove)
	{
		Move(DeltaTime);
	}

	//Rotate(DeltaTime);
}

void AMovableObject::SetShouldMove(const bool Move)
{
	ShouldMove = Move;
}

void AMovableObject::Move(float DeltaTime)
{
	const FVector CurrentLocation = GetActorLocation();
	const FVector NewLocation = FMath::Lerp(CurrentLocation, EndPoint, Velocity * DeltaTime);
	SetActorLocation(NewLocation);


	if (GetActorLocation() == EndPoint)
	{
		/*float OVerShoot = distanceMoved - distanceToMove;
		UE_LOG(LogTemp, Display, TEXT("I AM OVERSHOOT LOL %f"), OVerShoot)
		*/
		Arrived = true;
		StartPoint = CurrentLocation;
		Velocity *= -1;
	}
}

void AMovableObject::Rotate(float DeltaTime)
{
	//AddActorLocalRotation(DeltaTime * RotationVelocity);
}
