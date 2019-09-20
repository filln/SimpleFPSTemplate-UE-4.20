// Fill out your copyright notice in the Description page of Project Settings.

//Класс для AI-врага (бота)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

//Перечисление для состояний бота
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,		//Ожидает
	Suspicious, //Подозревает
	Alert		//Тревога
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Метод для перегрузки евента PawnSensingComp->OnSeePawn, сработает если увидел перса (игрока)
	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	//Метод для перегрузки евента PawnSensingComp->OnHearNoise, сработает если услышит перса
	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	//Возвращает бота в режим ожидания
	UFUNCTION()
		void ResetOrientation();

	//Евент для блюпринтов - если сменилось состояние
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChanged(EAIState newState);

	//Установить состояние бота
	void SetGuardState(EAIState newState);

	//Компонент для определения перса
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	//Дефолтный ротатор поворота бота
	FRotator OriginalRotation;

	//Таймер для выполнения ResetOrientation после некоторой паузы
	FTimerHandle TimerHandle_ResetOrientation;
	
	//Текущее состояние бота
	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
		EAIState guardState;

	//Выполняется на клиенте при репликации guardState на клиента
	UFUNCTION()
		void OnRep_GuardState();

//Патрулирование//////////////////////////////////////////////////////////////////////////
protected:

	//Патрулировать?
	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;

	//Первая точка патрулирования
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor *firstPatrolPoint;

	//Вторая точка патрулирования
	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor *secondPatrolPoint;

	//Текущая точка патрулирования
	AActor *currentPatrolPoint;

	//Двигаться к след. точке патрулирования
	void MoveToNextPatrolPoint();

	//Остановиться
	void StopMovement();

	//Проверка приближения к точке патрулирования и движение к этой точке
	void PatrolGoalChecks();

//////////////////////////////////////////////////////////////////////////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
