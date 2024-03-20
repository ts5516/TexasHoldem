// Fill out your copyright notice in the Description page of Project Settings.


#include "THWidget_RealLobby.h"

UTHWidget_RealLobby::UTHWidget_RealLobby(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTHWidget_RealLobby::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerTileView = Cast<UTileView>(GetWidgetFromName(TEXT("PlayerList")));
}
