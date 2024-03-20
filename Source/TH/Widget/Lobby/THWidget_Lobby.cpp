
#include "THWidget_Lobby.h"
#include "Components/TileView.h"

UTHWidget_Lobby::UTHWidget_Lobby(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UTHWidget_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	//GetGameState()->Delegate_UpdateRoomManager.AddUFunction(this, FName("OnUpdatedLobby"));

	RoomTileView = Cast<UTileView>(GetWidgetFromName(TEXT("RoomList")));

	InitLobby();
}

void UTHWidget_Lobby::OnUpdatedLobby()
{
	RenderRoomTileView();
}

void UTHWidget_Lobby::InitLobby()
{	
	if (GetGameState()->GetRoomManager())
	{
		RenderRoomTileView();
	}
}

void UTHWidget_Lobby::RenderRoomTileView()
{
	RoomTileView->SetListItems(GetGameState()->GetRooms());
}
