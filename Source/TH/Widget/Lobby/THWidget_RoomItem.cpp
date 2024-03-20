// Fill out your copyright notice in the Description page of Project Settings.


#include "THWidget_RoomItem.h"

UTHWidget_RoomItem::UTHWidget_RoomItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTHWidget_RoomItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	TObjectPtr<APlayerController> Player = Cast<APlayerController>(ListItemObject);

	if (Player != nullptr)
	{
		Name->SetText(FText::FromString(Player->GetName()));
	}
}
