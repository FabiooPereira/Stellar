// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Area.h"
#include "Landscape.h"
#include "EngineUtils.h"


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
}

void AEnemy_Area::SpawnDarkness()
{
	RemoveEnemy();
	if (DecalComponent)
	{
		DecalComponent->SetVisibility(true);
	}}

void AEnemy_Area::RemoveDarkness()
{
	SpawnEnemy();
	if (DecalComponent)
	{
		DecalComponent->SetVisibility(false);
	}}

#include "Enemy.h" // Include the header file of your Enemy class

void AEnemy_Area::SpawnEnemy()
{
	float GroundOffset = 90; // Adjust this as needed
    
	FHitResult Hit;

	
	if (PerformRaycast(Hit)) { 
		FRotator RandomRotation(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);
		FVector SpawnLocation = Hit.Location + FVector(0, 0, GroundOffset);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		CurrentEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, SpawnLocation, RandomRotation, SpawnParams);

		if (CurrentEnemy != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Enemy spawned"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Enemy not spawned"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Raycast failed or no landscape found"));
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


FVector AEnemy_Area::GetRandomPointInVolume(float ZOffset)
{
	float BorderOffset = 50; // Adjust the offset as needed
	
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
	FVector ActorLocation = GetActorLocation();

	// Generate random X and Y coordinates relative to the actor's current position
	float SpawnX = FMath::FRandRange(-BoxExtent.X+BorderOffset, BoxExtent.X-BorderOffset);
	float SpawnY = FMath::FRandRange(-BoxExtent.Y+BorderOffset, BoxExtent.Y-BorderOffset);
	float SpawnZ = ActorLocation.Z;

	//Raycast from the actor to the landscape
	FVector FinalLocation = ActorLocation + FVector(SpawnX, SpawnY, SpawnZ + ZOffset);
	return FinalLocation;
}

bool AEnemy_Area::PerformRaycast(FHitResult& OutHit)
{
	UWorld* world = GetWorld();
	if (world == nullptr) return false;

	// Find the active landscape
	TActorIterator<ALandscape> LandscapeIterator(world);
	if (!LandscapeIterator) return false; // Proper check for iterator validity
	ALandscape* Landscape = *LandscapeIterator;

	FCollisionQueryParams CollisionParams(FName(TEXT("FoliageClusterPlacementTrace")), true, this);
	CollisionParams.bReturnPhysicalMaterial = true;

	FVector StartLocation = GetRandomPointInVolume(1000);  // Assume 1000 units above the volume
	FVector EndLocation = GetRandomPointInVolume(-1000);   // Assume 1000 units below the volume

	return Landscape->ActorLineTraceSingle(OutHit, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
}

void AEnemy_Area::SpawnRock()
{
}

// Called every frame
void AEnemy_Area::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

