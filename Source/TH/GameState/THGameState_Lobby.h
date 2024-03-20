// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "THGameState.h"
#include "TH/GameInfo/THRoom.h"
#include "TH/Manager/RoomManager.h"
#include "THGameState_Lobby.generated.h"

DECLARE_MULTICAST_DELEGATE(FDelegate_UpdateRoomManager);

UCLASS()
class ATHGameState_Lobby : public ATHGameState
{
	GENERATED_BODY()

public:

	ATHGameState_Lobby();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_RoomManager)
	ARoomManager* RoomManager;

	UFUNCTION()
	void OnRep_RoomManager();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerIds)
	TArray<int32> PlayerIds;

	UFUNCTION()
	void OnRep_PlayerIds();

public:	// property replication

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void AddPlayer(APlayerController* Player);
	void RemovePlayer(APlayerController* Player);

	ATHRoom* CreateRoom(const FRoomInfo& RoomInfo);
	ATHRoom* EnterRoom(TObjectPtr<APlayerController> Player, const int32 RoomID);
	ATHRoom* ExitRoom(TObjectPtr<APlayerController> Player, const int32 RoomID);

	ATHRoom* GetRoomByID(const int32 RoomID) { return RoomManager->FindRoom(RoomID); }
	TArray<ATHRoom*> GetRooms() { return RoomManager->GetRooms(); }
	ARoomManager* GetRoomManager() { return RoomManager; }

public:

	FDelegate_UpdateRoomManager Delegate_UpdateRoomManager;
};