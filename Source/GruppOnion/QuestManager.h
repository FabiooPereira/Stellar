// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "QuestManager.generated.h"
class AQuest;
/**
 * 
 */

UCLASS()
class GRUPPONION_API AQuestManager : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString Description = "default description";

	UPROPERTY(EditAnywhere)
	AQuest* CurrentObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, AActor*> MapOfObjectives;

	void BeginNext();

	UFUNCTION(BlueprintCallable)
	bool IsCleared(FString Name);
};
