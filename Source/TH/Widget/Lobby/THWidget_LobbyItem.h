// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/TextBlock.h"
#include "THWidget_LobbyItem.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TH_API UTHWidget_LobbyItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

	UTHWidget_LobbyItem(const FObjectInitializer& ObjectInitializer);

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NumOfPeople;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Capacity;

public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};
