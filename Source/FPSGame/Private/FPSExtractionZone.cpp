// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Создать коллизию
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));

	//Задать параметры коллизии - без физики
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Игнорить все
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//Откликаться только на Pawn методом Overlap
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//Задать размер коллизии
	OverlapComp->SetBoxExtent(FVector(200.0f));

	//Сделать невидимым в игре
	OverlapComp->SetHiddenInGame(false);

	//Сделать коллизию рутом
	RootComponent = OverlapComp;

	//Задать евент при оверлапе
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	//Создать компонент частиц
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));

	//Задать размер области частиц
	DecalComp->DecalSize = FVector(200.0f);

	//Присоединить к руту
	DecalComp->SetupAttachment(RootComponent);
}


void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Запомнить найденного актора
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	//Если актор невалиден, то дальше ничего не делать
	if (MyPawn == nullptr) return;

	//Записать в лог событие сработки коллизии
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));

	//Если объект миссии найден
	if (MyPawn->bIsCarryingOjective)
	{
		//Взять гейммод с сервера
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			//Завершить миссию
			GM->CompleteMission(MyPawn, true);
		}
	}
	//Если объект миссии не найден
	else
	{
		//проиграть звук неудавшейся миссии
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}


