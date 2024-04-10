// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "QuestManager.generated.h"
class UQuest;
/**
 * 
 */
UCLASS()
class GRUPPONION_API AQuestManager : public AGameState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Description = "default description";

	UQuest* CurrentObjective;
	TArray<TSubclassOf<UQuest>*> Objectives;
};
