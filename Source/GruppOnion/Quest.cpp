// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
// Sets default values
AQuest::AQuest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuest::BeginQuest_Implementation()
{
}

void AQuest::EndQuest()
{
	Clear();
}

void AQuest::Clear()
{
	Cleared = true;
	UE_LOG(LogTemp, Error, TEXT("quest clear function"))
}

bool AQuest::IsCleared()
{
	return false;
}
