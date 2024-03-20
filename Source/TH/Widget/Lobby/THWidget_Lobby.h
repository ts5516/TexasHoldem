// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TH/GameState/THGameState_Lobby.h"
#include "THWidget_LobbyItem.h"
#include "THWidget_Lobby.generated.h"

class UTileView;

UCLASS(BlueprintType)
class TH_API UTHWidget_Lobby : public UUserWidget
{
	GENERATED_BODY()

public:

	UTHWidget_Lobby(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TObjectPtr<UTileView> RoomTileView;

public:

	void InitLobby();

	UFUNCTION()
	void OnUpdatedLobby();

	UFUNCTION(BlueprintCallable)
	void RenderRoomTileView();

public:

	ATHGameState_Lobby* GetGameState() const { return GetWorld()->GetGameState<ATHGameState_Lobby>(); }
};
