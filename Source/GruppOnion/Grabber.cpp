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
	if (GrabbedObject == nullptr)
	{
		// Check if there's an overlapping object
		if (HandCollision->GetNumOverlappingComponents() > 0)
		{
			// Assuming the first overlapping component is the object to grab
			UPrimitiveComponent* OverlappingComponent = HandCollision->GetOverlappingComponent(0);
			if (OverlappingComponent->IsSimulatingPhysics())
			{
				// Disable physics simulation for the object
				OverlappingComponent->SetSimulatePhysics(false);

				// Attach the object to the hand socket
				GrabbedObject = Cast<AActor>(OverlappingComponent->GetOwner());
				if (GrabbedObject)
				{
					// Attach the object to the hand socket
					GrabbedObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "SocketName");
				}
			}
		}
	}
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));
}