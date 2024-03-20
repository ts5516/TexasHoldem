// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "TH/GameInfo/THRoom.h"
#include "Components/ActorComponent.h"
#include "RoomManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FDelegate_UpdateRooms);

/**
 * 
 */
UCLASS()
class TH_API ARoomManager : public AInfo
{
	GENERATED_BODY()

public:

	ARoomManager();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Rooms)
	TArray<ATHRoom*> Rooms;

	UFUNCTION()
	void OnRep_Rooms();

public:	// property replication

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:

	int32 CreateRoom(const FRoomInfo& RoomInfo);
	void UpdateRoom(int32 RoomID, FRoomInfo UpdateRoomInfo);
	void RemoveRoom(int32 RoomID);

	void AddPlayerInRoom(TObjectPtr<APlayerController> Player, ATHRoom* Room);
	void RemovePlayerInRoom(TObjectPtr<APlayerController> Player, ATHRoom* Room);

	ATHRoom* FindRoom(int32 RoomID);

	TArray<ATHRoom*> GetRooms() { return Rooms; }

private:

	bool IsValidRoom(const FRoomInfo& RoomInfo);
	bool IsDuplicatedRoomName(const FString& RoomName);
	bool IsEmptyRoomName(const FString& RoomName);

public:

	FDelegate_UpdateRooms Delegate_UpdateRooms;
};
