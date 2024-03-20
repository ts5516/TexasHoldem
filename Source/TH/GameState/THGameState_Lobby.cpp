// Copyright Epic Games, Inc. All Rights Reserved.

#include "THGameState_Lobby.h"
#include "Net/UnrealNetwork.h"

ATHGameState_Lobby::ATHGameState_Lobby()
	: Super()
{
	//RoomManager = CreateDefaultSubobject<URoomManager>(TEXT("RoomManager"));
}

void ATHGameState_Lobby::BeginPlay()
{
	Super::BeginPlay();

	RoomManager = (ARoomManager*)GetWorld()->SpawnActor(ARoomManager::StaticClass());
}

void ATHGameState_Lobby::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->DestroyActor(RoomManager);

	Delegate_UpdateRoomManager.Clear();
}

void ATHGameState_Lobby::OnRep_RoomManager()
{
	if (Delegate_UpdateRoomManager.IsBound())
	{
		Delegate_UpdateRoomManager.Broadcast();
	}
}

void ATHGameState_Lobby::OnRep_PlayerIds()
{
	UE_LOG(LogTemp, Log, TEXT("Player Array is successfully replicated"));
}

void ATHGameState_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATHGameState_Lobby, RoomManager);
	DOREPLIFETIME(ATHGameState_Lobby, PlayerIds);
}

void ATHGameState_Lobby::AddPlayer(APlayerController* Player)
{
	check(HasAuthority());

	PlayerIds.Add(Player->GetUniqueID());
	UE_LOG(LogTemp, Log, TEXT("Player Array is successfully added"));
}

void ATHGameState_Lobby::RemovePlayer(APlayerController* Player)
{
	PlayerIds.Remove(Player->GetPlatformUserId());
}

ATHRoom* ATHGameState_Lobby::CreateRoom(const FRoomInfo& RoomInfo)
{
	check(HasAuthority());

	int32 RoomID = RoomManager->CreateRoom(RoomInfo);

	if (RoomID != NULL)
	{
		//OnRep_RoomManager();

		return RoomManager->FindRoom(RoomID);
	}

	return nullptr;
}

ATHRoom* ATHGameState_Lobby::EnterRoom(TObjectPtr<APlayerController> Player, const int32 RoomID)
{
	check(HasAuthority());

	ATHRoom* Room = RoomManager->FindRoom(RoomID);

	if (Room)
	{
		RoomManager->AddPlayerInRoom(Player, Room);
		return Room;
	}

	return nullptr;
}

ATHRoom* ATHGameState_Lobby::ExitRoom(TObjectPtr<APlayerController> Player, const int32 RoomID)
{
	check(HasAuthority());

	ATHRoom* Room = RoomManager->FindRoom(RoomID);

	if (Room)
	{
		RoomManager->RemovePlayerInRoom(Player, Room);
		return Room;
	}

	return nullptr;
}