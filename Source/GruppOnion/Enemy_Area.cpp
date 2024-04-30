// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Area.h"

// Sets default values
AEnemy_Area::AEnemy_Area()
{
	// Set default values
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the Decal Component
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("AreaDecal"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = DecalSize;
	DecalComponent->SetRelativeRotation(DecalRotation);  // Apply the rotation

	// Initialize the Box Collision Component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(BoxSize);
	BoxComponent->SetRelativeRotation(BoxRotation);
}


// Called when the game starts or when spawned
void AEnemy_Area::BeginPlay()
{
	Super::BeginPlay();
	SpawnRock();
}

void AEnemy_Area::SpawnDarkness()
{
	SpawnEnemy();
}

void AEnemy_Area::RemoveDarkness()
{
	RemoveEnemy();
}

#include "Enemy.h" // Include the header file of your Enemy class

void AEnemy_Area::SpawnEnemy()
{
	// Get the bounds of the box collider
	FVector BoxExtent = BoxComponent->GetScaledBoxExtent(); // Replace MyBoxCollider with the name of your box collider variable

	// Get the location of the box collider
	FVector BoxLocation = BoxComponent->GetComponentLocation(); // Replace MyBoxCollider with the name of your box collider variable

	// Generate a random location within the bounds of the box collider
	FVector RandomLocation = FMath::RandPointInBox(FBox(BoxLocation - BoxExtent, BoxLocation + BoxExtent));

	// Spawn the Enemy object at the random location
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, RandomLocation, FRotator::ZeroRotator); // Replace EnemyClass with the class of your Enemy object

	// Optionally, you can set up additional properties of the spawned Enemy here
	if (NewEnemy)
	{
		// Set any additional properties of the spawned Enemy
	}
}


void AEnemy_Area::RemoveEnemy()
{
}

void AEnemy_Area::SpawnRock()
{
}

// Called every frame
void AEnemy_Area::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

