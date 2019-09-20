// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AFPSHUD::AFPSHUD()
{
	//Найти ассет прицела 
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UI/FirstPersonCrosshair"));

	//Взять объект класса прицела
	CrosshairTex = CrosshairTexObj.Object;
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	//Найти центр Canvas (центр экрана)
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//Найти позицию, при которой центр прицела будет в центре экрана
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	//Задать объект класса прицела на экране
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);

	//Задать смешивание прицела с экраном
	TileItem.BlendMode = SE_BLEND_Translucent;

	//Отрисовать прицел
	Canvas->DrawItem( TileItem );
}
