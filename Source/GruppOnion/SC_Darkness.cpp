#include "SC_Darkness.h"

USC_Darkness::USC_Darkness()
{
	// Initialize component properties
	PrimaryComponentTick.bCanEverTick = true;
	bVisualizeComponent = true;

	// Create and attach the procedural mesh component
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ProceduralMesh->SetupAttachment(this);
}

void USC_Darkness::BeginPlay()
{
	Super::BeginPlay();
	CreateOrUpdateMesh();
}

void USC_Darkness::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Update mesh dynamically if necessary
}

void USC_Darkness::CreateOrUpdateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	// Define your mesh here based on your needs

	// Update the procedural mesh
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}