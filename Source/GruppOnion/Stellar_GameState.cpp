// Fill out your copyright notice in the Description page of Project Settings.


#include "Stellar_GameState.h"

void AStellar_GameState::UpdateNPCData(const FString& NPCName, int32 NewState, int32 TreeIndex ){
	if (FNPCData* InteractionData = NPCDataMap.Find(NPCName))
	{
		// Update existing entry
		InteractionData->InteractionState = NewState;
	}
	else
	{
		// Add new entry
		FNPCData NewInteractionData;
		NewInteractionData.bHasTalkedTo = true;
		NewInteractionData.InteractionState = NewState;
		NewInteractionData.TreeIndex = TreeIndex;
		NPCDataMap.Add(NPCName, NewInteractionData);
	}
}

bool AStellar_GameState::GetHasTalkedTo(const FString& NPCName) const
{
	if (const FNPCData* InteractionData = NPCDataMap.Find(NPCName))
	{
		return InteractionData->bHasTalkedTo;
	}
	return false;
}

int32 AStellar_GameState::GetInteractionStatus(const FString& NPCName) const
{
	if(const FNPCData* InteractionData = NPCDataMap.Find(NPCName))
	{
		return InteractionData->InteractionState;
	}
	else
	{
		return 0;
	}
}

int32 AStellar_GameState::GetTreeIndex(const FString& NPCName) const
{
	if(const FNPCData* InteractionData = NPCDataMap.Find(NPCName))
	{
		return InteractionData->TreeIndex;
	}
	else
	{
		return 0;
	}
}
