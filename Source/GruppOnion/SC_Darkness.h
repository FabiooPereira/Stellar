// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "SC_Darkness.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRUPPONION_API USC_Darkness : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USC_Darkness();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decal", meta = (InstanceEditable = "true", EditInline = "true"))
	UDecalComponent* DarknessDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	USplineComponent* DarknessSpline;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Procedural mesh component to represent the darkness
	UProceduralMeshComponent* ProceduralMesh;
	


	// Helper function to create or update the mesh
	void CreateOrUpdateMesh();
	void InitializeDecal();

};
