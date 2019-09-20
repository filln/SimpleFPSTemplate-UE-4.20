// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//Класс персонажа

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UPawnNoiseEmitterComponent;


UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	//Меш перса
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
		USkeletalMeshComponent* Mesh1PComponent;

	//Меш оружия
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		USkeletalMeshComponent* GunMeshComponent;

	//Камера
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* CameraComponent;

	//Компонент источника шума
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UPawnNoiseEmitterComponent* NoiseEmitterComponent;

public:
	AFPSCharacter();

	//Класс снаряда
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	//Звук выстрела
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;

	//Анимация стрельбы
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	//Объект найден?
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Gameplay")
	bool bIsCarryingOjective;

protected:
	
	//Стрельба
	void Fire();

	//Стрельба на сервере
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	//Двигаться вперед или назад
	void MoveForward(float Val);

	//Двигатся вправо или влево
	void MoveRight(float Val);

	//Установка упаравления персом
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	//Вернуть Меш
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }

	//Вернуть камеру
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

	virtual void Tick(float DeltaTime) override;
};

