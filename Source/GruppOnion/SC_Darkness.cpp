// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_Darkness.h"

// USC_Darkness constructor
USC_Darkness::USC_Darkness()
{
	// Set this component to tick every frame.
	PrimaryComponentTick.bCanEverTick = true;

	// Create the procedural mesh component and attach it.
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProceduralMesh->SetupAttachment(this);

	// Create and attach the spline component
	DarknessSpline = CreateDefaultSubobject<USplineComponent>(TEXT("DarknessSpline"));
	DarknessSpline->SetupAttachment(this); // Attach to the USC_Darkness component

}



// Called when the game starts
void USC_Darkness::BeginPlay()
{
	Super::BeginPlay();

	CreateOrUpdateMesh();
	InitializeDecal();

	
}


// Called every frame
void USC_Darkness::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USC_Darkness::CreateOrUpdateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	// Define your mesh here based on your needs

	// Update the procedural mesh
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void USC_Darkness::InitializeDecal()
{
	// Set decal size (width, height, depth)
	DarknessDecal->DecalSize = FVector(200.0f, 100.0f, 100.0f);
	
	// Set other properties such as fade duration and start distance
	DarknessDecal->FadeScreenSize = 0.01f;
	DarknessDecal->FadeStartDelay = 0.0f;
	DarknessDecal->FadeDuration = 10.0f;
}


