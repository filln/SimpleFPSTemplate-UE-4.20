// Fill out your copyright notice in the Description page of Project Settings.

//Класс зоны конца миссии. Когда перс достигает зоны, миссия пройдена

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"

class UBoxComponent;
class UDecalComponent;
class USoundBase;

UCLASS()
class FPSGAME_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExtractionZone();

protected:

	//Коллизиия для оверлапа
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OverlapComp;

	//Компонент частиц для выделения зоны оверлапа
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDecalComponent* DecalComp;

	//Звук неудавшейся миссии (объект не найден)
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* ObjectiveMissingSound;

	//Метод для перегрузки евента OverlapComp->OnComponentBeginOverlap
	UFUNCTION()
		void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
