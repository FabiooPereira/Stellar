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
	
}

// Called every frame
void AEnemy_Area::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

