// Fill out your copyright notice in the Description page of Project Settings.

#include "THPlayerController_Lobby.h"
#include "GameFramework/PlayerState.h"
#include "TH/Widget/Lobby/THWidget_Lobby.h"
#include "Net/UnrealNetwork.h"

ATHPlayerController_Lobby::ATHPlayerController_Lobby(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ATHPlayerController_Lobby::BeginPlay()
{
	Super::BeginPlay();

	GetGameState()->Delegate_UpdateRoomManager.AddUFunction(this, FName("OnCreateRoomManager"));
	/*ARoomManager* RoomManager = GetGameState()->GetRoomManager();
	if (RoomManager)
	{
		RoomManager->Delegate_UpdateRooms.AddUFunction(this, FName("OnUpdateRoomManager"));
	}*/
}	

void ATHPlayerController_Lobby::OnCreateRoomManager()
{
	GetGameState()->GetRoomManager()->Delegate_UpdateRooms.AddUFunction(this, FName("OnUpdateRooms"));
	
	//OnUpdateRooms();
}

void ATHPlayerController_Lobby::Server_EnterRoom_Implementation(ATHRoom* Room)
{
	if (Room == nullptr || GetPlayerState()->GetPlayerRoom())
	{
		return;
	}

	ATHRoom* PlayerRoom = GetGameState()->EnterRoom(this, Room->GetUniqueID());
	GetPlayerState()->SetPlayerRoom(PlayerRoom);

	if (PlayerRoom)
	{
		//PlayerRoomID = Room->GetUniqueID();
		//ATHRoom* PlayerRoom = GetGameState()->GetRoomByID(PlayerRoomID);

		UE_LOG(LogTemp, Log, TEXT("EnterRoom: %d"), PlayerRoom->GetData().Players.Num());
	}
}

void ATHPlayerController_Lobby::Server_ExitRoom_Implementation()
{
}

void ATHPlayerController_Lobby::Server_CreateRoom_Implementation(const FRoomInfo& RoomInfo)
{
	ATHGameState_Lobby* GameState = GetGameState();

	ATHRoom* PlayerRoom = GameState->CreateRoom(RoomInfo);
	GetPlayerState()->SetPlayerRoom(PlayerRoom);

	TArray<ATHRoom*> Rooms = GameState->GetRoomManager()->GetRooms();

	if (PlayerRoom)
	{
		for (auto Room : Rooms)
		{
			UE_LOG(LogTemp, Log, TEXT("GameState Room: %s"), *(Room->GetData().Title));

		}
		UE_LOG(LogTemp, Log, TEXT("PlayerController Room: %s"), *(PlayerRoom->GetData().Title));
	}
}

void ATHPlayerController_Lobby::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnUpdatePlayerState();
}

void ATHPlayerController_Lobby::Server_ReadyPlayer_Implementation(const FText& PlayerName)
{
	this->SetName(PlayerName.ToString());
	GetGameState()->AddPlayer(this);
}

ATHGameState_Lobby* ATHPlayerController_Lobby::GetGameState() const
{
	return  GetWorld() ? Cast<ATHGameState_Lobby>(GetWorld()->GetGameState()) : nullptr;
}

ATHPlayerState_Lobby* ATHPlayerController_Lobby::GetPlayerState() const
{
	return Cast<ATHPlayerState_Lobby>(PlayerState);
}

void ATHPlayerController_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ATHPlayerController_Lobby, PlayerRoom);
}