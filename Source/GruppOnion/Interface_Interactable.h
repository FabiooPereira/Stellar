// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GRUPPONION_API IInterface_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AActor* Instigator);
	virtual void Interact_Implementation(AActor* Instigator) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void ConversationClosed();
	virtual void ConversationClosed_Implementation() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void SetInteractableState(bool bCanShowInteract);
	virtual void SetInteractableState_Implementation(bool bCanShowInteract) = 0;
};
