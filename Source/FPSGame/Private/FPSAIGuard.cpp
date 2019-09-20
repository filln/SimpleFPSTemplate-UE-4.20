// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//Вызывать Tick() каждый кадр
	PrimaryActorTick.bCanEverTick = true;

	//Создать компонент
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//Работает только с Super::PostInitializeComponents()
//	Super::PostInitializeComponents();
//	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);

	//Состояние ожидания
	SetGuardState(EAIState::Idle);

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	//Перегрузить евенты компонента PawnSensingComp кастомными методами. Не работает в конструкторе без Super::PostInitializeComponents()
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	//Задать дефолтный ротатор
	OriginalRotation = GetActorRotation();

	//Если разрешено, то двигаться к след. точке патрулирования
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	//Состояние тревоги
	SetGuardState(EAIState::Alert);

	//Отрисовать дебаг-сферу
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	//Конец миссии при обнаружении перса. GetAuthGameMode() берет ГеймМод только с сервера, где он и есть
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	//Прекратить движение
	StopMovement();
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	//Если в уже в состоянии тревоги, то ничего не делать.
	if (guardState == EAIState::Alert)
	{
		return;
	}

	//Бот подозревает
	SetGuardState(EAIState::Suspicious);

	//Отрисовать дебаг-сферу
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	//Вектор, направленный к источнику шума
	FVector Direction = Location - GetActorLocation();

	//Нормализовать
	Direction.Normalize();

	//Найти ротатор вектора
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();

	//Оставить только Yaw
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	
	//Повернуть бота к источнику шума
	SetActorRotation(NewLookAt);

	//Перезапустить таймер
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	//Прекратить движение 
	StopMovement();
}

void AFPSAIGuard::ResetOrientation()
{
	//Если тревога, то ничего не делать
	if (guardState == EAIState::Alert)
	{
		return;
	}

	//Бот ожидает
	SetGuardState(EAIState::Idle);

	//Повернуть бота по дефолту
	SetActorRotation(OriginalRotation);

	//Если разрешено, то двигаться 
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::SetGuardState(EAIState newState)
{
	//Если состояние такое же, то ничего не делать
	if (guardState == newState)
	{
		return;
	}

	//Задать новое состояние
	guardState = newState;

	//Выполнить на клиенте
	OnRep_GuardState();
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(guardState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	//Если текущая точка патрулирования не задана или является второй точкой
	if (currentPatrolPoint == nullptr || currentPatrolPoint == secondPatrolPoint)
	{
		//первую точку назначить текущей
		currentPatrolPoint = firstPatrolPoint;
	}
	else
	{
		//иначе вторую чтоку назначить текущей
		currentPatrolPoint = secondPatrolPoint;
	}

	//Двигаться к текущей точке патрулирования
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), currentPatrolPoint);
}

void AFPSAIGuard::StopMovement()
{
	AController *controller = GetController();
	if (controller) 
	{
		controller->StopMovement();
	}
}

void AFPSAIGuard::PatrolGoalChecks()
{
	//Если текущая точка патрулирования задана
	if (currentPatrolPoint)
	{
		//Рассчитать расстояние до точки
		float distanceToGoal = (GetActorLocation() - currentPatrolPoint->GetActorLocation()).Size();

		//Если меньше 50, то двигаться к следующей
		if (distanceToGoal < 50)
		{
			MoveToNextPatrolPoint();
		}
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PatrolGoalChecks();

}

//Реплицировать данные
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSAIGuard, guardState);
}

