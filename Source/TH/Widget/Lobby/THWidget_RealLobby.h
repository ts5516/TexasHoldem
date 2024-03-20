// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TH/Widget/THWidget.h"
#include "THWidget_RealLobbyItem.h"
#include "THWidget_RealLobby.generated.h"

class UTileView;

/**
 * 
 */
UCLASS(BlueprintType)
class TH_API UTHWidget_RealLobby : public UTHWidget
{
	GENERATED_BODY()
	
public:

	UTHWidget_RealLobby(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TObjectPtr<UTileView> PlayerTileView;
};
