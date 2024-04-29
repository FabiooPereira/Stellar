// Fill out your copyright notice in the Description page of Project Settings.


#include "SC_Darkness.h"

// Sets default values for this component's properties
USC_Darkness::USC_Darkness()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bVisualizeComponent = true;

	// Create and attach the procedural mesh component
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProceduralMesh->SetupAttachment(this);
}


// Called when the game starts
void USC_Darkness::BeginPlay()
{
	Super::BeginPlay();

	CreateOrUpdateMesh();
	
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

