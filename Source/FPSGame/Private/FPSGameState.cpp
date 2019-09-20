// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "FPSPlayerController.h"

//Конец миссии, репликация на все клиенты. Вызывается на сервере.
void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	//Перебрать массив всех контроллеров игроков
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		//Взять текущий контроллер
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

		//Если контроллер валиден и является локальным
		if (PC && PC->IsLocalController())
		{
			//Конец миссии
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			//Взять подконтрольного павна
			APawn* MyPawn = PC->GetPawn();
			if (MyPawn)
			{
				//Выключить управление игроком
				MyPawn->DisableInput(PC);
			}
		}
	}
}
