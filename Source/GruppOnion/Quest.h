// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

UCLASS(Abstract)
class GRUPPONION_API AQuest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void BeginQuest();

	UFUNCTION(BlueprintCallable)
	virtual void EndQuest();

	UFUNCTION(BlueprintCallable)
	virtual void Clear();

	UFUNCTION(BlueprintCallable)
	virtual bool IsCleared();

	UPROPERTY(EditAnywhere)
	FString Description = "default description";

private:
	UPROPERTY(EditAnywhere)
	bool Cleared = false;
};
