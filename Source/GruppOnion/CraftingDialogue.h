// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CraftingDialogue.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRUPPONION_API UCraftingDialogue : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingDialogue();

	UPROPERTY(EditAnywhere, Category="Audio", DisplayName="Use Sound?")
	bool bUseAudio = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio", DisplayName="Audio Component")
	UAudioComponent* Acomp = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void OpenCraftMenu();

	UFUNCTION(BlueprintCallable)
	void CloseCraftMenu();

	UFUNCTION(BlueprintCallable)
	void LoadAudio();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
