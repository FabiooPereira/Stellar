// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"

#include "Kismet/GameplayStatics.h"

void UAudioManager::PlaySoundAtLocation(USoundBase* AudioClip, FVector Location)
{
	if (AudioClip)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AudioClip, Location);
	}
}
