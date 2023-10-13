#include "ActionRoguelike/Public/Pawns/GF_CasterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

AGF_CasterCharacter::AGF_CasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildClass();
}

void AGF_CasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddInputMapping();
}

void AGF_CasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDisplayDebugInfo) DebugCharacter();
}

void AGF_CasterCharacter::BuildClass()
{
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("Camera Spring Arm");
	CameraSpringArm->SetupAttachment(RootComponent);
	
	CharacterCamera = CreateDefaultSubobject<UCameraComponent>("Character Camera");
	CharacterCamera->SetupAttachment(CameraSpringArm);

	baseCharacterSpeed = GetCharacterMovement()->MaxWalkSpeed;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	CameraSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
}

void AGF_CasterCharacter::AddInputMapping()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem) Subsystem->AddMappingContext(CharacterInputMappingContext, 0);
	}
}

void AGF_CasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputs = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputs->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::MoveForward);
		EnhancedInputs->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::MoveRight);
		EnhancedInputs->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::Rotate);
		EnhancedInputs->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::LookUp);
		EnhancedInputs->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputs->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);
		EnhancedInputs->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputs->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::Sprint);
		EnhancedInputs->BindAction(IA_Sprint, ETriggerEvent::Canceled, this, &AGF_CasterCharacter::StopSprinting);
		EnhancedInputs->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AGF_CasterCharacter::StopSprinting);
		EnhancedInputs->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &AGF_CasterCharacter::ShootPrimaryWeapon);
	}
}

void AGF_CasterCharacter::MoveForward(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	FRotator Rotation = GetControlRotation();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;
	AddMovementInput(Rotation.Vector(), AxisValue.Y);
}

void AGF_CasterCharacter::MoveRight(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	FRotator Rotation = GetControlRotation();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;
	FVector Direction = UKismetMathLibrary::GetRightVector(Rotation);
	AddMovementInput(Direction, AxisValue.X);
}

void AGF_CasterCharacter::Rotate(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	AddControllerYawInput(AxisValue.X);
}

void AGF_CasterCharacter::LookUp(const FInputActionValue& Value)
{
	const FVector2D AxisValue = Value.Get<FVector2D>();
	AddControllerPitchInput(AxisValue.Y);
}

void AGF_CasterCharacter::Sprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AGF_CasterCharacter::StopSprinting(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = baseCharacterSpeed;
}

void AGF_CasterCharacter::ShootPrimaryWeapon(const FInputActionValue& Value)
{
	FTransform SpawnTransform = FTransform(GetControlRotation(), GetActorLocation());
	FActorSpawnParameters SpawnParamenter;
	SpawnParamenter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParamenter);
}

void AGF_CasterCharacter::DebugCharacter()
{
	const float DrawScale = 100.0f;
	const float Thickness = 2.0f;

	FVector LineStart = GetActorLocation();
	LineStart += GetActorRightVector() * 80.0f;
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 60.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow,false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 60.0f);
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false,0.0f, 0, Thickness);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, TEXT("----------------------"));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, TEXT("--> Controller Direction"));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, TEXT("--> Actor Direction"));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, TEXT("::Direction Arrows::"));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, TEXT("----------------------"));
	
	// GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf("Movement Speed: %d", GetCharacterMovement()->MaxWalkSpeed));

}