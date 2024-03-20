// Fill out your copyright notice in the Description page of Project Settings.


#include "THWidget_RealLobbyItem.h"

UTHWidget_RealLobbyItem::UTHWidget_RealLobbyItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTHWidget_RealLobbyItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	TObjectPtr<APlayerController> Player = Cast<APlayerController>(ListItemObject);

	if (Player != nullptr)
	{
		PlayerName->SetText(FText::FromString(Player->GetName()));
	}
}
