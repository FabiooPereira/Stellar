// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"

#include "Quest.h"

void AQuestManager::BeginPlay()
{
	Super::BeginPlay();

	for (auto a : Objectives)
	{
		if (!a)
		{
			UE_LOG(LogTemp, Error, TEXT("name is null"))
		}
		else if (!Cast<AQuest>(a))
		{
			UE_LOG(LogTemp, Error, TEXT("Non-quest object in quest array! Its name is: %s"),
			       *(a->GetActorNameOrLabel()))
		}
		else
		{
			if (!CurrentObjective)
			{
				CurrentObjective = Cast<AQuest>(a);
			}
			//UE_LOG(LogTemp, Error, TEXT("Quest objects name is  %s"), *(a->GetActorNameOrLabel()))
		}
	}
	BeginNext();
}

void AQuestManager::BeginNext()
{
	if (CurrentObjective)
	{
		for (int32 i = 0; i < Objectives.Num(); i++)
		{
			if (Objectives[i])
			{
				if (Objectives[i] == CurrentObjective)
				{
					AQuest* tmp = Cast<AQuest>(Objectives[i++]);
					if (tmp)
					{
						CurrentObjective = tmp;
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Actor is null"))
						return;
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Actor is null"))
				return;
			}
		}
	}

	if (CurrentObjective)
	{
		CurrentObjective->BeginQuest();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Current Objective is null"))
	}
}

bool AQuestManager::IsCleared(FString Name)
{
	if (Name.IsEmpty() || !MapOfObjectives.Contains(Name))
	{
		return false;
	}

	AQuest* tmp = Cast<AQuest>(MapOfObjectives[Name]);
	if (!tmp)
	{
		return false;
	}
	return tmp->IsCleared();
}
