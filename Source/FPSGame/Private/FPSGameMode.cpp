// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	//Найти объект блюпринт-перса
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));

	//Взять класс объекта
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//Назначить HUD
	HUDClass = AFPSHUD::StaticClass();

	//Назначить GameState
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	//Если нет павна, то ничего не делать дальше
	if (!InstigatorPawn)
	{
		return;
	}

	//Если конечная точка камеры определена
	if (SpectatingViewpointClass)
	{
		//Объявить массив акторов для найденных акторов класса SpectatingViewpointClass
		TArray<AActor*> ReturnedActors;

		//Найти акторов класса 
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

		//Если массив не пустой
		if (ReturnedActors.Num() > 0)
		{
			//Взять первый элемент массива
			AActor* NewViewTarget = ReturnedActors[0];

			//Перебрать массив всех контроллеров игроков
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				//Взять текущий контроллер
				APlayerController* PC = It->Get();
				if (PC)
				{
					//Переместить камеру 
					PC->SetViewTargetWithBlend(NewViewTarget, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
	}
	//Если конечная точка камеры не определена
	else
	{
		//Вывести предупреждение в лог
		UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass is nullptr. Please update GameMode class with valid subclass. Cannot change spectating view target."));
	}

	//Взять геймстейт
	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		//Конец миссии (отправка всем клиентам)
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	//Конец миссии (евент для блюпринтов)
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
