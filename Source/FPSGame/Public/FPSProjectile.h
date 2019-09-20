// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//Класс снаряда

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"


class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class AFPSProjectile : public AActor
{
	GENERATED_BODY()

protected:

	/** Sphere collision component */
	//Коллизия 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	//Компонент движения снаряда
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

public:

	AFPSProjectile();

	/** called when projectile hits something */
	//Функция перегрузки CollisionComp->OnComponentHit. Срабатывает, когда снаряд ударяется обо что-нибудь
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	//Геттер для указателя на коллизию
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	//Геттер для указателя компонент движения снаряда
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

