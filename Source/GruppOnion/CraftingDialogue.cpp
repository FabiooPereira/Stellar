// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingDialogue.h"

// Sets default values for this component's properties
UCraftingDialogue::UCraftingDialogue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UCraftingDialogue::BeginPlay()
{
	Super::BeginPlay();

	// Check if we should use audio
	if (bUseAudio)
	{
		// Get the owner of this component
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			// Create and attach a new audio component to the owner
			Acomp = NewObject<UAudioComponent>(MyOwner);
			if (Acomp)
			{
				Acomp->AttachToComponent(MyOwner->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				Acomp->RegisterComponent();
				
				// If you need to set any default properties for the AudioComponent, do that here
				// For example:
				// Acomp->bAutoActivate = false; // Prevents the AudioComponent from playing immediately
			}
		}
	}
}


// Called every frame
void UCraftingDialogue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UCraftingDialogue::OpenCraftMenu()
{
	
}
void UCraftingDialogue::CloseCraftMenu()
{
	
}

void UCraftingDialogue::LoadAudio()
{
	
}





