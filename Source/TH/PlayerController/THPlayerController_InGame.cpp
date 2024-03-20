// Fill out your copyright notice in the Description page of Project Settings.


#include "THPlayerController_InGame.h"

void ATHPlayerController_InGame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto _PlayerState = GetPlayerState();

	if (_PlayerState)
	{
		if (bUpdateWidget)
		{
			UTHCard* Card1 = _PlayerState->GetPlayerCards()[0];
			UTHCard* Card2 = _PlayerState->GetPlayerCards()[1];

			UpdateHUD(Card1, Card2);

			bUpdateWidget = false;
		}
		if (bUpdateHUD)
		{
			bool result = _PlayerState->GetIsMyTurn();

			SetEnableBettingButton(result);

			bUpdateHUD = false;
		}
		if (bUpdateCommunityCards)
		{
			TArray<UTHCard*> Cards = GetGameState()->GetCommunityCards();

			UpdateCommunityCards(Cards);

			bUpdateCommunityCards = false;
		}
		if (bUpdatePlayerBankroll)
		{
			UpdatePlayerBankroll(_PlayerState->GetBankroll());

			bUpdatePlayerBankroll = false;
		}
	}
}

bool ATHPlayerController_InGame::IsCorrectRaiseMoney(int32 RaiseMoney)
{
	int32 MinRaiseMoney = GetGameState()->GetMinRaiseMoney();

	return RaiseMoney >= MinRaiseMoney;
}

void ATHPlayerController_InGame::Server_BetPlayer_Implementation()
{
	GetPlayerState()->SetBetMyTurn(true);
	GetPlayerState()->SetBetOnce(true);
}

void ATHPlayerController_InGame::Server_ReadyPlayer_Implementation()
{
	GetPlayerState()->SetReadyToPlay(true);
	//GetGameState()->AddPokerplayerArray(GetPlayerState());
}

void ATHPlayerController_InGame::Server_Betting_Implementation(int32 BettingAmount, FName Action)
{	
	if (GetPlayerState()->GetBankroll() == 0)
	{
		GetPlayerState()->SetCanBet(false);
	}

	GetGameState()->BettingFromPlayer(GetPlayerState()->GetFName(), BettingAmount, Action);
}

void ATHPlayerController_InGame::Server_FoldPlayer_Implementation()
{
	GetPlayerState()->SetCanBet(false);
	GetPlayerState()->SetIsFold(true);

	GetGameState()->BettingFromPlayer(GetPlayerState()->GetFName(), 0, TEXT("Fold"));
}

void ATHPlayerController_InGame::Server_AllinPlayer_Implementation()
{
	GetPlayerState()->SetCanBet(false);
	GetPlayerState()->SetBetMyTurn(true);
	GetPlayerState()->SetBetOnce(true);
}

void ATHPlayerController_InGame::Client_ShowPlayerCards_Implementation(const TArray<UTHCard*>& Cards, const TArray<bool>& IsPutFrontCards)
{
	UpdateTableCards(Cards, IsPutFrontCards);
}

void ATHPlayerController_InGame::Client_ShowGameResult_Implementation(const TArray<FName>& CardNames, int32 GameResult)
{
	UpdateGameResult(CardNames, GameResult);
}

void ATHPlayerController_InGame::Client_ShowPlayerBetting_Implementation(const TArray<int32>& Bettings, const TArray<FName>& Actions)
{
	UpdateAllPlayerBettings(Bettings, Actions);
}

void ATHPlayerController_InGame::Client_ShowPlayerInfo_Implementation(const TArray<FName>& Roles)
{
	UpdatePlayerRoles(Roles);
}

void ATHPlayerController_InGame::Client_InitUI_Implementation(const TArray<bool>& CardVisibles)
{
	InitUI(CardVisibles);
}

void ATHPlayerController_InGame::Client_ShowGameEndMessage_Implementation()
{
	ShowGameEndMessage();
}
