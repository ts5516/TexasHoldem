// Fill out your copyright notice in the Description page of Project Settings.


#include "THGameMode_Title.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ATHGameMode_Title::TravelToLobby(int32 ControllerId)
{
	UWorld* World = GetWorld();
	APlayerController* PC = UGameplayStatics::GetPlayerController(World, ControllerId);

	if (World && PC)
	{
		bUseSeamlessTravel = true;
		PC->ClientTravel(FString("/Game/Maps/Lobby"), TRAVEL_Relative);
	}
}
