// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GF_CasterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ACTIONROGUELIKE_API AGF_CasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGF_CasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
	UCameraComponent* CharacterCamera;

	UPROPERTY(EditAnywhere, Category = "Class Config")
	bool bDisplayDebugInfo = false;
	
	UPROPERTY(EditAnywhere, Category = "Class Config")
	float SprintSpeed = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* CharacterInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_Movement;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_Shoot;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_Sprint;

	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AActor> ProjectileClass;

	
protected:
	virtual void BeginPlay() override;
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprinting(const FInputActionValue& Value);
	void ShootPrimaryWeapon(const FInputActionValue& Value);
	

private:
	float baseCharacterSpeed;
	
private:
	void BuildClass();
	void DebugCharacter();
};
