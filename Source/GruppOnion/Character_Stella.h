// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GruppOnionCharacter.h"
#include "Components/SphereComponent.h"
#include "Character_Stella.generated.h"

/**
 * 
 */
UCLASS()
class GRUPPONION_API ACharacter_Stella : public AGruppOnionCharacter
{
	GENERATED_BODY()

public:
	ACharacter_Stella();
	
	void InteractWithObject(AActor* Other);
	void Disableinteract(AActor* Other);
	void SetInteractable(AActor* Other);
	void LockPlayerMovement();
	void UnlockPlayerMovement();

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* InteractionSphere;

private:
	UPROPERTY()
	AActor* InteractableObject;

};


