// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THPlayerState.h"
#include "TH/GameInfo/THRoom.h"
#include "THPlayerState_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class TH_API ATHPlayerState_Lobby : public ATHPlayerState
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void OnRep_PlayerRoom();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerInRoom() { return PlayerRoom != nullptr; }

	UFUNCTION()
	void SetPlayerRoom(ATHRoom* NewPlayerRoom) { PlayerRoom = NewPlayerRoom; }

	UFUNCTION(BlueprintCallable)
	ATHRoom* GetPlayerRoom() { return PlayerRoom; }

private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerRoom)
	ATHRoom* PlayerRoom;

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
