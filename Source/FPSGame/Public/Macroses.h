#pragma once
#include "Engine/GameEngine.h"

#define DEBUGMESSAGE(CustomString) UE_LOG(LogTemp, Log, TEXT(CustomString)); if(GEngine) {GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(CustomString));}