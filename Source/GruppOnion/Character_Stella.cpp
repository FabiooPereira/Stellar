// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Stella.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Interface_Interactable.h"
#include "GameFramework/Actor.h"


ACharacter_Stella::ACharacter_Stella()
{
	// Create a sphere component and attach it to the root component
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(100.f);  // Set default radius
	InteractionSphere->SetRelativeLocation(FVector(100.f, 0.f, 0.f));  // Set default position
}

void ACharacter_Stella::InteractWithObject(AActor* Other)
{
}

void ACharacter_Stella::Disableinteract(AActor* Other)
{
}

void ACharacter_Stella::SetInteractable(AActor* Other)
{
	if(Other && Other != this && Other->GetClass()->ImplementsInterface(UInterface_Interactable::StaticClass()))
	{

        IInterface_Interactable* InteractableInterface = Cast<IInterface_Interactable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->SetInteractableState(true);
			UE_LOG(LogTemp, Warning, TEXT("CAN INTERACT"));

		}
	}
}

void ACharacter_Stella::LockPlayerMovement()
{
}

void ACharacter_Stella::UnlockPlayerMovement()
{
}

void ACharacter_Stella::BeginPlay()
{
	Super::BeginPlay();

	// Additional BeginPlay code for ACharacter_Stella
}

void ACharacter_Stella::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Your additional bindings for ACharacter_Stella
}


void ACharacter_Stella::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != this)
	{
		SetInteractable(OtherActor);
	}
}


