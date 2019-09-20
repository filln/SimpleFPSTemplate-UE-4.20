// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	//Создать коллизию
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));

	//Задать дефолтный радиус коллизии
	CollisionComp->InitSphereRadius(5.0f);

	//Задать профиль коллизии (профиль создается в редакторе)
	CollisionComp->SetCollisionProfileName("Projectile");

	//Задать евент оверлапа коллизии
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	//Перс не может ходить по снаряду(???)
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	//Назначить рутом
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	//Создать компонент движения снаряда
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	//К чему применять движение
	ProjectileMovement->UpdatedComponent = CollisionComp;

	//Начальная скорость
	ProjectileMovement->InitialSpeed = 3000.f;

	//Максимальная скорость
	ProjectileMovement->MaxSpeed = 3000.f;

	//Разрешить вращать
	ProjectileMovement->bRotationFollowsVelocity = true;

	//Разрешить отскакивать
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//Время жизни снаряда
	InitialLifeSpan = 3.0f;

	//Реплицировать
	SetReplicates(true);

	//Реплицировать движение
	SetReplicateMovement(true);
}


void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	//Если пойманный актор валиден, не равен этому актору, пойманный компонент валиден и с включенной физикой
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//Применить импульс к пойманному компоненту
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	}

	//Если это сервер
	if (Role == ROLE_Authority)
	{
		//Создать шум
		MakeNoise(1.0f, Instigator);

		//Уничтожить снаряд
		Destroy();
	}
}