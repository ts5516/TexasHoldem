// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomManager.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

ARoomManager::ARoomManager()
	:Super()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	//bReplicateUsingRegisteredSubObjectList = true;
}

void ARoomManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Delegate_UpdateRooms.Clear();
}

void ARoomManager::OnRep_Rooms()
{
	if (Delegate_UpdateRooms.IsBound())
	{
		Delegate_UpdateRooms.Broadcast();
	}
}

void ARoomManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomManager, Rooms);
}

bool ARoomManager::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto Room : Rooms)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Room, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

int32 ARoomManager::CreateRoom(const FRoomInfo& RoomInfo)
{
	if (IsValidRoom(RoomInfo))
	{
		ATHRoom* NewRoom = (ATHRoom*)GetWorld()->SpawnActor(ATHRoom::StaticClass());
		NewRoom->SetData(RoomInfo);
		Rooms.Add(NewRoom);
		
		//AddReplicatedSubObject(NewRoom);

		return NewRoom->GetUniqueID();
	}

	return NULL;
}

void ARoomManager::UpdateRoom(int32 RoomID, FRoomInfo UpdateRoomInfo)
{
	
}

void ARoomManager::RemoveRoom(int32 RoomID)
{
	ATHRoom* Room = FindRoom(RoomID);

	if (Room)
	{
		Rooms.Remove(Room);
		GetWorld()->DestroyActor(Room);
	}
}

bool ARoomManager::IsValidRoom(const FRoomInfo& RoomInfo)
{
	if (IsDuplicatedRoomName(RoomInfo.Title))
	{
		return false;
	}
	else if (IsEmptyRoomName(RoomInfo.Title))
	{
		return false;
	}

	return true;
}

bool ARoomManager::IsDuplicatedRoomName(const FString& RoomName)
{
	for (ATHRoom* Room : Rooms)
	{
		if (RoomName.Equals(Room->GetData().Title))
		{
			return true;
		}
	}

	return false;
}

bool ARoomManager::IsEmptyRoomName(const FString& RoomName)
{
	FString Name = RoomName.TrimStartAndEnd();

	if (Name.IsEmpty())
	{
		return true;
	}

	return false;
}

void ARoomManager::AddPlayerInRoom(TObjectPtr<APlayerController> Player, ATHRoom* Room)
{
	FRoomInfo RoomData = Room->GetData();

	if (RoomData.Players.Num() < RoomData.Capacity)
	{
		Room->AddPlayer(Player);
	}
}

void ARoomManager::RemovePlayerInRoom(TObjectPtr<APlayerController> Player, ATHRoom* Room)
{
	Room->RemovePlayer(Player);

	if (Room->GetData().Players.IsEmpty())
	{
		RemoveRoom(Room->GetUniqueID());
	}	
}

ATHRoom* ARoomManager::FindRoom(int32 RoomID)
{
	if (!RoomID)
	{
		return nullptr;
	}

	for (auto IterRoom : Rooms)
	{
		if (RoomID == IterRoom->GetUniqueID())
		{
			return IterRoom;
		}
	}

	return nullptr;
}