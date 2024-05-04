// Copyright Epic Games, Inc. All Rights Reserved.

#include "GruppOnionCharacter.h"

#include "AICompanionController.h"
#include "AlpacaMovement.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGruppOnionCharacter

AGruppOnionCharacter::AGruppOnionCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}

void AGruppOnionCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	//Find reference to Companion(tag) and sets its controller to currentAIController;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("AICompanion"), FoundActors);
	if(FoundActors.Num()>0)
	{
		AActor* AICompanionActor= FoundActors[0];
		CurrentCompanion = AICompanionActor;
		AAICompanionController* CompanionController = Cast<AAICompanionController>(AICompanionActor->GetInstigatorController());
		if(CompanionController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Succes"))
			CurrentAIController = CompanionController;
		}
	}
	
	
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGruppOnionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGruppOnionCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGruppOnionCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGruppOnionCharacter::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemplateCharacter, Log, TEXT("Move function called."));

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CustomCamera != nullptr)
	{
		FRotator CameraRotation = CustomCamera->GetComponentRotation();
		FVector ForwardDirection =FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
		
		// // find out which way is forward
		// const FRotator Rotation = Controller->GetControlRotation();
		// const FRotator YawRotation(0, Rotation.Yaw, 0);
		//
		// // get forward vector
		// const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//
		// // get right vector 
		// const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGruppOnionCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	
}
//---------------------------CompanionCalls---------------------------------------//
void AGruppOnionCharacter::StayCompanion()
{
	if (CurrentAIController)
	{
		CurrentAIController->StopFollowPlayer();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("NUllptr"))
	}
	
}
void AGruppOnionCharacter::CallCompanion()
{
	
	if (CurrentAIController)
	{
	CurrentAIController->ContinueFollowPlayer();
	}else
	{
		UE_LOG(LogTemp,Warning,TEXT("NUllptr"));
	}
	
}
void AGruppOnionCharacter::setCompanion(AAlpacaMovement* NewCompanion)
{
	//CurrentCompanion = NewCompanion;
}
///////////////////////////////////////////////

void AGruppOnionCharacter::DrawDebugLineToCompanion()
{
	if(CurrentCompanion)
	{
		FVector PlayerLocation = GetActorLocation();

		FVector CompanionLocation = CurrentCompanion->GetActorLocation();

		DrawDebugLine(
			GetWorld(),
			PlayerLocation,
			CompanionLocation,
			FColor::Green,
			false,
			2.0f,
			0,
			5.0f
			);
	}
}

///////////////////////////////////////////////optional
void AGruppOnionCharacter::MarkTargetPosition(float MaxTraceDistance)
{
	FVector TargetPosition = CaptureTargetLocation(MaxTraceDistance);
}
FVector AGruppOnionCharacter::CaptureTargetLocation(float MaxTraceDistance)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller not found."))
		return FVector::ZeroVector;
	}
	// Get the camera location and rotation
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector EndLocation = CameraLocation + (CameraRotation.Vector() * MaxTraceDistance);

	// Perform a trace to capture the target location
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this); // Ignore the player character
	if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECC_Visibility, TraceParams))
	{
		// Target location captured successfully
		FVector TargetPosition = HitResult.ImpactPoint;

		// Draw debug line from player's viewpoint to target position
		DrawDebugLine(GetWorld(), CameraLocation, TargetPosition, FColor::Green, false, 2.0f, 0, 2.0f);
		return HitResult.ImpactPoint;
	}
	else
	{
		// No valid target location found
		UE_LOG(LogTemp, Warning, TEXT("No valid target location found."));
		return FVector::ZeroVector;
	}
}

void AGruppOnionCharacter::setActiveCamera(UCameraComponent* newCamera)
{
	CustomCamera = newCamera;
}