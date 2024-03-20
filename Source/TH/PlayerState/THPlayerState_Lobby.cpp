// Fill out your copyright notice in the Description page of Project Settings.

#include "THPlayerState_Lobby.h"
#include "TH/PlayerController/THPlayerController_Lobby.h"
#include "Net/UnrealNetwork.h"


void ATHPlayerState_Lobby::OnRep_PlayerRoom()
{
	ATHPlayerController_Lobby *PlayerController = Cast<ATHPlayerController_Lobby>(GetOwningController());
	PlayerController->OnUpdatePlayerState();
}

void ATHPlayerState_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATHPlayerState_Lobby, PlayerRoom);
}
