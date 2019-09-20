// Fill out your copyright notice in the Description page of Project Settings.

//Лаунчпад - при соприкосновении перса с ним перс летит вперед

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;


UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	//Меш
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	//Коллизия для оверлапа перса
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OverlapComp;

	//Сила запуска 
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LaunchStrength;

	//Угол запуска
	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
		float LaunchPitchAngle;

	//Частицы запуска лаунчпада
	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		UParticleSystem* ActivateLaunchPadEffect;

	//Функция перегрузки OverlapComp->OnComponentBeginOverlap. Запускает перса в полет
	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
