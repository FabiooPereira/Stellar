// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Area.h"
#include "Landscape.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "Components/DecalComponent.h"


// Sets default values
AEnemy_Area::AEnemy_Area()
{
	// Set default values
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the Decal Component
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("AreaDecal"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = DecalSize;
	DecalComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));  // Center it
	DecalComponent->SetRelativeRotation(DecalRotation);  // Apply the rotation

	// Initialize the Box Collision Component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(BoxSize);
	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));  // Center it
	BoxComponent->SetRelativeRotation(BoxRotation);
}


// Called when the game starts or when spawned
void AEnemy_Area::BeginPlay()
{
	Super::BeginPlay();

	if(DecalComponent)
	{
		// Get the existing material from the decal component
		UMaterialInterface* CurrentMaterial = DecalComponent->GetDecalMaterial();

		// Create a dynamic material instance from the existing material
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);

		// Set the new dynamic material to the decal component
		DecalComponent->SetDecalMaterial(DynamicMaterial);

		// Set a scalar parameter on the new dynamic material instance
		DynamicMaterial->SetScalarParameterValue(FName("Erosion"), 0.0f);
	}
}

void AEnemy_Area::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	do
	{
		FRotator RandomRotation(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);
		FVector SpawnLocation = GetRandomPointOnLandscape();
		UE_LOG(LogTemp, Warning, TEXT("Try Spawn"));
		CurrentEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, SpawnLocation, RandomRotation, SpawnParams);
	} while (!CurrentEnemy);

		if (CurrentEnemy != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Enemy spawned"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Enemy not spawned"));
		}
}

void AEnemy_Area::RemoveEnemy()
{
	if (CurrentEnemy != nullptr) {
		CurrentEnemy->EndPlay(EEndPlayReason::Destroyed);
		CurrentEnemy = nullptr; // Nullify the pointer after using it
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to remove a non-existent enemy"));
	}
}

AEnemy* AEnemy_Area::GetCurrentEnemy() const
{
	return CurrentEnemy;
}

FVector AEnemy_Area::GetThrowLocation(FVector OriginalVector, FVector EnemyLocation, float RangeFromCenter)
{
    FVector DirectionVector = EnemyLocation - OriginalVector;
    DirectionVector.Normalize();
	FVector ScaledDirection = DirectionVector * RangeFromCenter;
	FVector ThrowLocation = OriginalVector + ScaledDirection;
	
	return ThrowLocation;
}

FVector AEnemy_Area::GetRandomPointOnLandscape() const
{
	FHitResult OutHit;
	UWorld* World = GetWorld();
	ALandscape* Landscape = World->SpawnActor<ALandscape>();
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
	
	// Generate random X and Y coordinates within the box extent with border offset
	FVector RandomOffset = FVector(
		FMath::FRandRange(-BoxExtent.X + BorderOffset, BoxExtent.X - BorderOffset),
		FMath::FRandRange(-BoxExtent.Y + BorderOffset, BoxExtent.Y - BorderOffset),
		GetActorLocation().Z
	) + GetActorLocation();
    
	// Define start and end locations for the raycast
	FVector StartLocation = RandomOffset + FVector(0,0,1000);
	FVector EndLocation = RandomOffset + FVector(0,0,-1000);

	// Set up collision query parameters
	FCollisionQueryParams CollisionParams(FName(TEXT("GetPointOnLandscape")), true, this);
	
	// Perform the raycast
	
	if(World->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_WorldStatic, CollisionParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Raycast Success"));
		return OutHit.Location + FVector(0, 0, GroundOffset);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Raycast Failed"));
		return FVector(0,0,0);
	}
		

	// Draw debug line
	//FColor LineColor = bHitSuccess ? FColor::Green : FColor::Red; // Green if hit, red if no hit
	//DrawDebugLine(World, StartLocation, EndLocation, LineColor, false, 5, 0, 5.0f);
	
}

// Called every frame
void AEnemy_Area::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

