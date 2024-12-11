
#include "Character/OTW_Character.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AOTW_Character::AOTW_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AOTW_Character::BeginPlay()
{
	Super::BeginPlay();
}


void AOTW_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOTW_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Input Mapping
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	// Input Action
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOTW_Character::jump);

		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOTW_Character::Move);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOTW_Character::Look);
	}

}

void AOTW_Character::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// Directions
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Forward Direction
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//Right Direction
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Add Movement
		AddMovementInput(Forward, InputVector.Y);
		AddMovementInput(Right, InputVector.X);
	}
}

void AOTW_Character::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void AOTW_Character::jump()
{
	ACharacter::Jump();
}

