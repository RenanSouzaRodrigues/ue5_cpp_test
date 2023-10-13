#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GF_Projectile.generated.h"

class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API AGF_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AGF_Projectile();
	virtual void Tick(float DeltaTime) override;

	
protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Visuals")
	UParticleSystem* ProjectileParticles;

	
protected:
	virtual void BeginPlay() override;


private:
	void BuildClass();
	
};
