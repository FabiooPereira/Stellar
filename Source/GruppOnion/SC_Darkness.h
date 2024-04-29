// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ProceduralMeshComponent.h"
#include "SC_Darkness.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRUPPONION_API USC_Darkness : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Constructor
	USC_Darkness();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Procedural mesh component to represent the darkness
	UProceduralMeshComponent* ProceduralMesh;

	// Helper function to create or update the mesh
	void CreateOrUpdateMesh();
};
