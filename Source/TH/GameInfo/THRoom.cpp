// Fill out your copyright notice in the Description page of Project Settings.

#include "THRoom.h"
#include "Net/UnrealNetwork.h"

ATHRoom::ATHRoom()
	:Super()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateUsingRegisteredSubObjectList = true;
}

void ATHRoom::SetData(const FRoomInfo& NewRoomData)
{
	RoomData.Title = NewRoomData.Title;
	//RoomData.NumOfPeople = NewRoomData.NumOfPeople;
	RoomData.Capacity = NewRoomData.Capacity;
	RoomData.SetPlayers(NewRoomData.Players);
}

void ATHRoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATHRoom, RoomData);
}

bool ATHRoom::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto Player : RoomData.Players)
	{
		bWroteSomething |= Channel->ReplicateSubobject(Player, *Bunch, *RepFlags);
	}

	return bWroteSomething;
}

bool ATHRoom::IsSupportedForNetworking() const
{
	return true;
}
