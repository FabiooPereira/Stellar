// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Stellar_GameState.generated.h"


// Struct for storing NPC interaction data
USTRUCT(BlueprintType)
struct FNPCData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHasTalkedTo = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 InteractionState = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TreeIndex = 0;
};


UCLASS()
class GRUPPONION_API AStellar_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	// Update the NPC interaction data
	UFUNCTION(BlueprintCallable, Category = "NPC Interaction")
	void UpdateNPCData(const FString& NPCName, int32 NewState = 0, int32 TreeIndex = 0);

	// Get if the player has talked to the NPC
	UFUNCTION(BlueprintCallable, Category = "NPC Interaction")
	bool GetHasTalkedTo(const FString& NPCName) const;

	// Get the interaction status of the NPC
	UFUNCTION(BlueprintCallable, Category = "NPC Interaction")
	int32 GetInteractionStatus(const FString& NPCName) const;

	UFUNCTION(BlueprintCallable, Category = "NPC Interaction")
	int32 GetTreeIndex(const FString& NPCName) const;

private:

	// Map of NPC data by name
	UPROPERTY()
	TMap<FString, FNPCData> NPCDataMap;
};
