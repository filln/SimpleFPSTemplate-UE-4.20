// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Net/UnrealNetwork.h"

AFPSCharacter::AFPSCharacter()
{
	//Создать камеру
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	//Присоединить к капсуле
	CameraComponent->SetupAttachment(GetCapsuleComponent());

	//Установить положение камеры в положение центра капсулы
	CameraComponent->RelativeLocation = FVector(0, 0, BaseEyeHeight); 

	//Поворачивать перса за поворотом камеры
	CameraComponent->bUsePawnControlRotation = true;

	// Создать меш
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));

	//Присоединить меш к камере
	Mesh1PComponent->SetupAttachment(CameraComponent);

	//Отключить тень меша
	Mesh1PComponent->CastShadow = false;

	//Относительный поворот меша
	Mesh1PComponent->RelativeRotation = FRotator(2.0f, -15.0f, 5.0f);

	//Относительное положение меша
	Mesh1PComponent->RelativeLocation = FVector(0, 0, -160.0f);

	//Создать меш оружия
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));

	//Отключить тень оружия
	GunMeshComponent->CastShadow = false;

	//Присоединить оружие к мешу перса
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");

	//Создать компонент источника шума
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComponent"));
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Дебаг-сообщение об невалидном PlayerInputComponent
	check(PlayerInputComponent);

	//Клавиатура
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Мышь
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Если контроллер локальный (не сетевой)
	if (!IsLocallyControlled())
	{
		//Относительный ротатор
		FRotator NewRot = CameraComponent->RelativeRotation;

		//Задать вращение вокруг Y с помощью RemoteViewPitch - реплицируемой переменной вращения камеры.
		NewRot.Pitch = RemoteViewPitch * 360.0f / 255.0f;

		//Установить относительное вращение камеры
		CameraComponent->SetRelativeRotation(NewRot);
	}


}

void AFPSCharacter::Fire()
{
	ServerFire();

	//Проиграть звук выстрела
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	//Проиграть анимацию выстрела
	if (FireAnimation)
	{
		//Взять объект анимации
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}


void AFPSCharacter::ServerFire_Implementation()
{
	if (ProjectileClass)
	{
		//Взять вектор сокета
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");

		//Взять ротатор сокета
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");

		//Параметры спавна
		FActorSpawnParameters ActorSpawnParams;

		//Параметр спавна коллизии - не спавниться, если есть пересечения коллизий
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		//Источник спавна
		ActorSpawnParams.Instigator = this;

		//Спавнить снаряд
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
}


bool AFPSCharacter::ServerFire_Validate()
{
	return true;
}

void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		//Двигаться в данном направлении
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		//Двигаться в данном направлении
		AddMovementInput(GetActorRightVector(), Value);
	}
}

//Реплицировать данные
void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSCharacter, bIsCarryingOjective);
	//DOREPLIFETIME_CONDITION(AFPSCharacter, bIsCarryingOjective, COND_OwnerOnly);
}
