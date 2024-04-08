// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(physicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Got physics handle: %s"), *physicsHandle->GetName());
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle found"));
	}
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(physicsHandle == nullptr)
	{
		return;
	}

	FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;
	physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	// ...
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(physicsHandle == nullptr)
	{
		return;
	}
	
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGrabDistance;
	DrawDebugLine(GetWorld(), start, end, FColor::Red);
	DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Blue, false, 5);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(grabRadius);
	FHitResult hitResult;
	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if(hasHit)
	{
		DrawDebugSphere(GetWorld(), hitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);

		physicsHandle->GrabComponentAtLocationWithRotation(hitResult.GetComponent(), NAME_None, hitResult.ImpactPoint, hitResult.GetComponent()->GetComponentRotation());
	}
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}