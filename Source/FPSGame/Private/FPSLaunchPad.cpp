// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	LaunchStrength = 1500;
	LaunchPitchAngle = 35;

	//Создать коллизию
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));

	//Задать размеры коллизии
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));

	//Назначить рутом
	RootComponent = OverlapComp;

	//Создать меш
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	//Присоединить к руту
	MeshComp->SetupAttachment(RootComponent);

	//Перегрузить евент OverlapComp->OnComponentBeginOverlap функцией OverlapLaunchPad - срабатывает по оверлапу с персом
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Базовый ротатор для направления запуска перса
	FRotator LaunchDirection = GetActorRotation();

	//Задать угол запуска перса
	LaunchDirection.Pitch += LaunchPitchAngle;

	//Задать вектор запуска перса
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	//Получить чарактера, оверлапнутого коллизией
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);

	//Если валиден
	if (OtherCharacter)
	{
		//Придать скорость персу
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);

		//Спавнить частицы эффекта запуска
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
	//Если чарактер невалиден, то проверить на валидность входного актора и включена ли физика у него
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		//Придать скорость (импульс) актору
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		//Спавнить частицы эффекта запуска
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
	}
}


