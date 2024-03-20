// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THPlayerController.h"
#include "TH/GameState/THGameState_Lobby.h"
#include "TH/PlayerState/THPlayerState_Lobby.h"
#include "THPlayerController_Lobby.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TH_API ATHPlayerController_Lobby : public ATHPlayerController
{
	GENERATED_BODY()
	
public:
	
	ATHPlayerController_Lobby(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

//-----------------------------------
// About Room
public:

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_EnterRoom(ATHRoom* Room);

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_ExitRoom();

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_CreateRoom(const FRoomInfo& RoomInfo);

	//UFUNCTION(BlueprintImplementableEvent, Category = "Lobby Event")
	//void OnRep_PlayerRoom();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby Event")
	void OnUpdatePlayerState();

	UFUNCTION()
	void OnCreateRoomManager();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby Event")
	void OnUpdateRooms();

	virtual void OnRep_PlayerState() override;
private:

	//UPROPERTY(ReplicatedUsing = OnRep_PlayerRoom)
	//int32 PlayerRoomID;
//-----------------------------------

//----------------------------------- Player
public:
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_ReadyPlayer(const FText& PlayerName);
//-----------------------------------
public:

	ATHGameState_Lobby* GetGameState() const;

	UFUNCTION(BlueprintCallable)
	ATHPlayerState_Lobby* GetPlayerState() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
