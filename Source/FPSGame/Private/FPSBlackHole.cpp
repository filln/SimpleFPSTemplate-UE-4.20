// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//Вызывать Tick() каждый кадр
	PrimaryActorTick.bCanEverTick = true;

	//Создать меш
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	//Отключить коллизии меша
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Назначить рутом
	RootComponent = MeshComp;

	//Создать внутреннюю коллизию
	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));

	//Установить радиус сферы
	InnerSphereComponent->SetSphereRadius(100);

	//Присоединить к мешу
	InnerSphereComponent->SetupAttachment(MeshComp);

	//Перегрузить евент InnerSphereComponent->OnComponentBeginOverlap методом OverlapInnerSphere
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	//Создать внешнюю коллизию
	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));

	//Устнаовить радиус сферы
	OuterSphereComponent->SetSphereRadius(3000);

	//Присоединить к мешу
	OuterSphereComponent->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Уничтожить актор 
	if (OtherActor) OtherActor->Destroy();
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Создать массив пересекшихся компонентов
	TArray<UPrimitiveComponent*> OverlappingComp;

	//Заполнить массив
	OuterSphereComponent->GetOverlappingComponents(OverlappingComp);

	//Перебор компонентов
	for (int32 i = 0; i < OverlappingComp.Num(); i++)
	{
		//Взять компонент из массива
		UPrimitiveComponent* PrimComp = OverlappingComp[i];

		//Если компонент валиден и с физикой
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			//Взять радиус внешней коллизии с учетом масштабирования
			const float SphereRadius = OuterSphereComponent->GetScaledSphereRadius();

			//Задать параметр силы 
			const float ForceStrength = -2000;

			//Применить силу к компоненту
			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

