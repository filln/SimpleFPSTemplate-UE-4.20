// Fill out your copyright notice in the Description page of Project Settings.

//Предмет, который нужно взять для завершения миссии

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class AFPSCharacter;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Меш
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	//Коллизия
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	//Эффекты предмета - частицы.
	void PlayEffect();

	//Частицы эффекта предмета
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickUpFX;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Перегрузка евента оверлапа с предметом
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:

	//Перс, завладевший предметом
	AFPSCharacter* MyCharacter;
};
