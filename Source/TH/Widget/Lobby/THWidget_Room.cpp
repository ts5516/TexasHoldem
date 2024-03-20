#include "THWidget_Room.h"
#include "TH/GameState/THGameState_Lobby.h"
#include "Components/TileView.h"

UTHWidget_Room::UTHWidget_Room(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTHWidget_Room::NativeConstruct()
{
	Super::NativeConstruct();

	//GetGameState()->Delegate_UpdateRoomManager.AddUFunction(this, FName("OnUpdatedRoom"));

	Title = Cast<UTextBlock>(GetWidgetFromName(TEXT("Title")));
	NumOfPeople = Cast<UTextBlock>(GetWidgetFromName(TEXT("NumOfPeople")));
	Capacity = Cast<UTextBlock>(GetWidgetFromName(TEXT("Capacity")));
	PlayerTileView = Cast<UTileView>(GetWidgetFromName(TEXT("PlayerTileView")));

	InitRoom();
}

void UTHWidget_Room::OnUpdatedRoom()
{
	
}

void UTHWidget_Room::RenderRoom(ATHRoom* Room)
{
	Title->SetText(FText::FromString(Room->GetData().Title));
	NumOfPeople->SetText(FText::FromString(FString::FromInt(Room->GetData().Players.Num())));
	Capacity->SetText(FText::FromString(FString::FromInt(Room->GetData().Capacity)));
	PlayerTileView->SetListItems(Room->GetData().Players);
}

void UTHWidget_Room::InitRoom()
{

}
