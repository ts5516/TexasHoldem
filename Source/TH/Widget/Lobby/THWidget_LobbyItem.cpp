// Fill out your copyright notice in the Description page of Project Settings.

#include "THWidget_LobbyItem.h"
#include "TH/GameInfo/THRoom.h"

UTHWidget_LobbyItem::UTHWidget_LobbyItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UTHWidget_LobbyItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	ATHRoom* Room = Cast<ATHRoom>(ListItemObject);

	if (Room != nullptr)
	{
		Title->SetText(FText::FromString(Room->GetData().Title));
		NumOfPeople->SetText(FText::FromString(FString::FromInt(Room->GetData().Players.Num())));
		Capacity->SetText(FText::FromString(FString::FromInt(Room->GetData().Capacity)));
	}
}
