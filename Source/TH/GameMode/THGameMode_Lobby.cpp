// Fill out your copyright notice in the Description page of Project Settings.


#include "THGameMode_Lobby.h"
#include "TH/GameState/THGameState_Lobby.h"
#include "TH/PlayerController/THPlayerController_Lobby.h"

ATHGameMode_Lobby::ATHGameMode_Lobby()
	: Super()
{
	bUseSeamlessTravel = true;

	GameStateClass = ATHGameState_Lobby::StaticClass();
	PlayerControllerClass = ATHPlayerController_Lobby::StaticClass();
}

void ATHGameMode_Lobby::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
