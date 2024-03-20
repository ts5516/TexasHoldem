// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "THWidget_RealLobbyItem.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TH_API UTHWidget_RealLobbyItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UTHWidget_RealLobbyItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
