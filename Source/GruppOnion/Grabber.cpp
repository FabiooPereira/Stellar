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

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Got physics handle: %s"), *PhysicsHandle->GetName());
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

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	physicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	// ...
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent *PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle == nullptr)
	{
		return;
	}

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Cyan, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if(HasHit)
	{
		FVector ImpactPoint = HitResult.Location - (Start - End).GetSafeNormal() * Sphere.GetSphereRadius();
		AActor* HitActor = HitResult.GetActor();
		UE_LOG(LogTemp, Display, TEXT("hit actor: %s"), *HitActor->GetActorNameOrLabel());
		DrawDebugSphere(GetWorld(), ImpactPoint, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);

		PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}else
	{
		UE_LOG(LogTemp, Display, TEXT("no actor hit"));

	}
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}