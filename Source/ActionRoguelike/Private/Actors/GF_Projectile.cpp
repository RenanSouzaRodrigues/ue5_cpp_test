#include "Actors/GF_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"


AGF_Projectile::AGF_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGF_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void AGF_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGF_Projectile::BuildClass()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileParticles = CreateDefaultSubobject<UParticleSystem>(TEXT("ProjectileParticles"));

	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;
}