// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "InventorySlotWidget.h"
#include "InventorySystem/Public/InventoryWidget.h"
#include "InventorySystem/Public/GameScreenHUD.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/SizeBox.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInventorySystemCharacter


AInventorySystemCharacter::AInventorySystemCharacter()
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

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventorySystemCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AInventorySystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Look);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AInventorySystemCharacter::ToggleInventory);

		EnhancedInputComponent->BindAction(HotbarSelect1, ETriggerEvent::Started, this, &AInventorySystemCharacter::SelectHotbarSlot1);
		EnhancedInputComponent->BindAction(HotbarSelect2, ETriggerEvent::Started, this, &AInventorySystemCharacter::SelectHotbarSlot2);
		EnhancedInputComponent->BindAction(HotbarSelect3, ETriggerEvent::Started, this, &AInventorySystemCharacter::SelectHotbarSlot3);
		EnhancedInputComponent->BindAction(HotbarSelect4, ETriggerEvent::Started, this, &AInventorySystemCharacter::SelectHotbarSlot4);
		EnhancedInputComponent->BindAction(HotbarSelect5, ETriggerEvent::Started, this, &AInventorySystemCharacter::SelectHotbarSlot5);

		EnhancedInputComponent->BindAction(UseHotbarItemAction, ETriggerEvent::Started, this, &AInventorySystemCharacter::UseActiveItemAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInventorySystemCharacter::SelectHotbarSlot1() { SelectHotbarSlot(0); }
void AInventorySystemCharacter::SelectHotbarSlot2() { SelectHotbarSlot(1); }
void AInventorySystemCharacter::SelectHotbarSlot3() { SelectHotbarSlot(2); }
void AInventorySystemCharacter::SelectHotbarSlot4() { SelectHotbarSlot(3); }
void AInventorySystemCharacter::SelectHotbarSlot5() { SelectHotbarSlot(4); }

void AInventorySystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		GameHUD = Cast<AGameScreenHUD>(PC->GetHUD());
		if (GameHUD)
		{
			GameHUD->BindInventory(this);
		}
	}
}

void AInventorySystemCharacter::ToggleInventory()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!PC) return;
	
	if (bIsInventoryOpen)
	{
		GameHUD->InventoryWidget->ToggleInventoryVisibility();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(GameHUD->InventoryWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		GameHUD->InventoryWidget->ToggleInventoryVisibility();

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}

void AInventorySystemCharacter::SelectHotbarSlot(int32 Index)
{
	if (!InventoryComp) return;

	InventoryComp->ActiveHotbarIndex = Index;
	InventoryComp->OnInventoryUpdated.Broadcast();
}

void AInventorySystemCharacter::UseActiveItemAction()
{
	if (!InventoryComp) return;
	InventoryComp->UseActiveItem();
}



void AInventorySystemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventorySystemCharacter::Look(const FInputActionValue& Value)
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
