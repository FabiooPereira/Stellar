// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "Enemy.h"
#include "Enemy_Area.generated.h"

UCLASS()
class GRUPPONION_API AEnemy_Area : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_Area();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION(BlueprintCallable, Category="Darkness")
	void SpawnDarkness();
	
	UFUNCTION(BlueprintCallable, Category="Darkness")
	void RemoveDarkness();
	
	UFUNCTION(BlueprintCallable, Category="Enemy")
	void SpawnEnemy();
	
	UFUNCTION(BlueprintCallable, Category="Enemy")
	void RemoveEnemy();
	
	FVector GetRandomPointOnLandscape() const;

	void SpawnRock();

	UPROPERTY(EditAnywhere, Category = "Border Config")
	float BorderOffset = 50; // Adjust if needed, margin for border margin when spawning 

	UPROPERTY(EditAnywhere, Category = "Border Config")
	float GroundOffset = 90; // Adjust if needed, margin for Z axis when spawning
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotation;
	
	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInterface* DecalMaterial;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	FVector BoxSize;

	UPROPERTY(EditAnywhere, Category = "Components")
	FRotator BoxRotation;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemy> Enemy;

	AEnemy* CurrentEnemy = nullptr;

};
