// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableObject.generated.h"

UCLASS()
class GRUPPONION_API AMovableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Moving Platform")
	FVector EndPoint;

	UPROPERTY(EditAnywhere, Category="Moving Platform")
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere, Category="Moving Platform")
	float Velocity = 1;

	UFUNCTION(BlueprintCallable)
	void SetShouldMove(bool Move);

	/*
		UPROPERTY(EditAnywhere, Category="Rotate")
		FRotator RotationVelocity;
	*/

private:
	void Move(float DeltaTime);
	void Rotate(float DeltaTime);
	FVector StartPoint;
	bool Arrived = false;
};
