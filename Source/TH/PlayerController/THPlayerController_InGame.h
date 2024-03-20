// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THPlayerController.h"
#include "TH/GameState/THGameState_InGame.h"
#include "TH/PlayerState/THPlayerState_InGame.h"
#include "THPlayerController_InGame.generated.h"

/**
 * 
 */
UCLASS()
class TH_API ATHPlayerController_InGame : public ATHPlayerController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	ATHGameState_InGame* GetGameState() const
	{
		return  GetWorld() ? Cast<ATHGameState_InGame>(GetWorld()->GetGameState()) : nullptr;
	}

	UFUNCTION(BlueprintCallable)
	ATHPlayerState_InGame* GetPlayerState() const
	{
		return Super::GetPlayerState<ATHPlayerState_InGame>();
	}

	UFUNCTION(BlueprintCallable)
	bool IsCorrectRaiseMoney(int32 RaiseMoney);

	void SetUpdateWidget(bool bIsUpdated) { bUpdateWidget = bIsUpdated; }
	void SetUpdateHUD(bool bIsUpdated) { bUpdateHUD = bIsUpdated; }
	void SetUpdateCommunityCards(bool bIsUpdated) { bUpdateCommunityCards = bIsUpdated; }
	void SetUpdatePlayerNames(bool bIsUpdated) { bUpdatePlayerNames = bIsUpdated; }
	void SetUpdateBankroll(bool bIsUpdated) { bUpdatePlayerBankroll = bIsUpdated; }
public:

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_BetPlayer();

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_ReadyPlayer();

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_Betting(int32 BettingAmount, FName Action);

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_FoldPlayer();

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void Server_AllinPlayer();

	UFUNCTION(Client, Reliable)
	void Client_ShowPlayerCards(const TArray<UTHCard*>& Cards, const TArray<bool>& IsPutFrontCards);

	UFUNCTION(Client, Reliable)
	void Client_ShowGameResult( const TArray<FName>& CardNames, int32 GameResult);

	UFUNCTION(Client, Reliable)
	void Client_ShowPlayerBetting(const TArray<int32>& Bettings, const TArray<FName>& Actions);

	UFUNCTION(Client, Reliable)
	void Client_ShowPlayerInfo(const TArray<FName>& Roles);

	UFUNCTION(Client, Reliable)
	void Client_InitUI(const TArray<bool>& CardVisibles);

	UFUNCTION(Client, Reliable)
	void Client_ShowGameEndMessage();

	UFUNCTION(BlueprintImplementableEvent)
	void InitUI(const TArray<bool>& CardVisibles);

	UFUNCTION(BlueprintImplementableEvent)
	void InitHUD(bool MyCardVisible);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerNames(const TArray<FName>& Roles);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD(UTHCard* Card1, UTHCard* Card2);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCommunityCards(const TArray<UTHCard*>& Cards);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerRoles(const TArray<FName>& Roles);

	UFUNCTION(BlueprintImplementableEvent)
	void SetEnableBettingButton(bool Enable);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTableCards(const TArray<UTHCard*>& Cards, const TArray<bool>& IsPutFrontCards);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGameResult(const TArray<FName>& Cards, int32 GameResult);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerBankroll(int32 Bankroll);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAllPlayerBettings(const TArray<int32>& Bettings, const TArray<FName>& Actions);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameEndMessage();

private:

	bool bUpdateWidget = false;
	bool bUpdateHUD = false;
	bool bUpdateCommunityCards = false;
	bool bUpdatePlayerNames = false;
	bool bUpdatePlayerBankroll = false;
};
