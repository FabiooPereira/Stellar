// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlpacaMovement.generated.h"

UCLASS()
class GRUPPONION_API AAlpacaMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	AAlpacaMovement();
	UPROPERTY(EditAnywhere)
	float MovementSpeed = 300;
	UPROPERTY(EditAnywhere)
	FVector TargetDestination;
	ACharacter* PlayerCharacterptr;
	UPROPERTY(EditAnywhere)
	float FollowDistance = 5;
	UPROPERTY(EditAnywhere)
	bool IsFollowing = true;
	
	void MoveToLocation( FVector& TargetLocation,const float DeltaTime);
	void FollowCharacter(const ACharacter* CharacterToFollow,const float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void Stay();
	UFUNCTION(BlueprintCallable)
	void Follow();

	
	void SendToLocation();
private:
	void DrawDebugOffset(const ACharacter* Player)const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
