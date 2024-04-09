// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class GRUPPONION_API UAudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void PlaySoundAtLocation(USoundBase* AudioClip, FVector Location);
};
