// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"


// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Создать меш
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	//Отключить коллизии меша
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Назначить рутом
	RootComponent = MeshComp;

	//Создать коллизию
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));

	//Задать тип коллизии
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Задать реакцию на все - игнор
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	//Задать реакцию на павна - оверлап
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//Присоединить к мешу
	SphereComp->SetupAttachment(MeshComp);

	//Назначить реплицируемой
	SetReplicates(true);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffect();
}

void AFPSObjectiveActor::PlayEffect()
{
	//Спавнить частицы
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpFX, GetActorLocation());
}

// Called every frame
void AFPSObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffect();

	//Если выполняется на сервере
	if (Role == ROLE_Authority)
	{
		//Задать владельца предметом
		MyCharacter = Cast<AFPSCharacter>(OtherActor);

		//Если валиден
		if (MyCharacter)
		{
			//Установить флаг у владельца, что он владеет предметом
			MyCharacter->bIsCarryingOjective = true;

			//Уничтожить предмет
			Destroy();
		}
	}
}

