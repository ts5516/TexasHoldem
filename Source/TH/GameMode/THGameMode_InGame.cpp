// Fill out your copyright notice in the Description page of Project Settings.

#include "THGameMode_InGame.h"
#include "GameFramework/PlayerState.h"
#include "TH/GameState/THGameState_InGame.h"
#include "TH/PlayerState/THPlayerState_InGame.h"
#include "TH/PlayerController/THPlayerController_InGame.h"


ATHGameMode_InGame::ATHGameMode_InGame()
	:Super()
{
	GameStateClass = ATHGameState_InGame::StaticClass();
	SetRoundStage(RoundStage::None);
}

void ATHGameMode_InGame::InitGameState()
{
	Super::InitGameState();
}

void ATHGameMode_InGame::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/*auto _GameState = GetGameState<ATHGameState_InGame>();
	_GameState->AddPlayerName(NewPlayer->GetFName());*/

}

void ATHGameMode_InGame::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	//_GetGameState()->TestBankroll();
	//_GetGameState()->RoundInitTime = 10000;
	GoNextRoundStage();
}

void ATHGameMode_InGame::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsThePlayerBetting)
	{
		if (!bReadyToBetting)
		{
			ReadyToBetting();
		}

		StartBetting();

		if (!(IsValidBettingSituation()))
		{
			GoNextRoundStage();

			SetbIsThePlayerBetting(false);
			SetbReadyToBetting(false);
		}
	}
	else 
	{
		ProcessByRoundStage();

		if (GetRoundStage() == RoundStage::Flop 
			|| GetRoundStage() == RoundStage::PreFlop
			|| GetRoundStage() == RoundStage::Turn 
			|| GetRoundStage() == RoundStage::River)
		{
			SetbIsThePlayerBetting(true);
		}
	}
}

void ATHGameMode_InGame::ReadyToBetting()
{
	//_GetGameState()->InitTableMoney();
	_GetGameState()->InitPlayerActions();
	_GetGameState()->SetMinRaiseMoney(0);

	_GetGameState()->InitTurnPlayer();
	_GetGameState()->SetTurnPlayer(GetRoundStage());
	SetbReadyToBetting(true);

	if (GetRoundStage() == RoundStage::PreFlop)
	{
		_GetGameState()->BettingBlindMoney();
	}
}

void ATHGameMode_InGame::InitGameRound()
{
	_GetGameState()->InitGameInfo();
	_GetGameState()->InitRoundPlayer();
}

int32 ATHGameMode_InGame::GetMinBettingMoney(TArray<int32> PlayerMoney)
{
	int32 MinBettingMoney = INT32_MAX;

	for (int32 Money : PlayerMoney)
	{
		if (MinBettingMoney > Money && Money > 0)
		{
			MinBettingMoney = Money;
		}
	}

	return MinBettingMoney;
}

TArray<ATHPlayerState_InGame*> ATHGameMode_InGame::GetBestPlayers(TArray<ATHPlayerState_InGame*> Players)
{
	TArray<ATHPlayerState_InGame*> THPlayerArray;

	for (auto Player : Players)
	{
		if (Player->GetIsFold() || !(Player->GetCanGameplay()))
		{
			continue;
		}

		THPlayerArray.Add(Player);
	}

	FPokerHand BestHand;
	TArray<ATHPlayerState_InGame*> BestPlayers;

	for (auto THPlayer : THPlayerArray)
	{
		if (Players.IsEmpty())
		{
			BestHand = THPlayer->GetBestHand();
			BestPlayers.Add(THPlayer);
		}
		else
		{
			int32 Result = _GetGameState()->IsBetterThanFormerHand(BestHand, THPlayer->GetBestHand());

			if (Result == 0)
			{
				BestPlayers.Add(THPlayer);
			}
			else if (Result < 0)
			{
				BestPlayers.Empty();

				BestHand = THPlayer->GetBestHand();
				BestPlayers.Add(THPlayer);
			}
		}
	}

	return BestPlayers;
}

void ATHGameMode_InGame::StartBetting()
{
	auto Player = Cast<ATHPlayerState_InGame>(_GetGameState()->PlayerArray[_GetGameState()->GetTurnPlayerIndex()]);

	if (!(Player->GetBetMyTurn()))
	{
		return;
	}
	
	Player->SetIsMyTurn(false);
	Player->SetBetMyTurn(false);

	_GetGameState()->SetNextTurnPlayerIndex();
}

void ATHGameMode_InGame::ProcessByRoundStage()
{
	if (GetRoundStage() == RoundStage::PreFlop)
	{
		_GetGameState()->RoundInitTime = 15;
		RoundCount++;
		UE_LOG(LogTemp, Log, TEXT("Round %d Start"), RoundCount);
		_GetGameState()->SetPlayerRoles();

		/*TArray<FName> TestList =
		{
			TEXT("Dia3"), TEXT("Dia4") ,
			TEXT("Heart3"), TEXT("Heart4") ,
			TEXT("Club3"), TEXT("Club4"),
			TEXT("ClubA"),TEXT("ClubK"),TEXT("ClubQ"),TEXT("ClubJ"),TEXT("Club10"),
		};
		_GetGameState()->TestCards(TestList);*/

		_GetGameState()->ShuffleCards();
		_GetGameState()->DivideCardToPlayer();
	}
	else if (GetRoundStage() == RoundStage::Flop)
	{
		_GetGameState()->OpenFlopCards();
	}
	else if (GetRoundStage() == RoundStage::Turn || GetRoundStage() == RoundStage::River)
	{
		_GetGameState()->OpenCard();
	}
	else if (GetRoundStage() == RoundStage::Showdown)
	{
		DecideWinner();
		CalculateTableMoney();

		_GetGameState()->ShowPlayerCards();
		_GetGameState()->InitTurnPlayer();

		GoNextRoundStage();

		_GetGameState()->RoundTimerStart();

		UE_LOG(LogTemp, Log, TEXT("Round %d End"), RoundCount);
	}
	else if (GetRoundStage() == RoundStage::Waiting)
	{
		if (_GetGameState()->IsEndedWaiting())
		{
			InitGameRound();

			if (_GetGameState()->GetGameablePlayerNum() <= 1)
			{
				_GetGameState()->ShowGameEndMessage();
				SetRoundStage(RoundStage::None);
			}
			else
			{
				GoNextRoundStage();
			}
		}
	}

}

bool ATHGameMode_InGame::IsValidBettingSituation()
{
	bool Result = true;

	if (!IsPlayersLeftToBet())
	{
		Result = false;
	}

	int32 NotFoldPlayerNum = _GetGameState()->GetGameablePlayerNum() - _GetGameState()->GetFoldPlayerNum();
	if(NotFoldPlayerNum <= 1)
	{
		Result = false;
	}

	int32 BettablePlayerNum = _GetGameState()->GetGameablePlayerNum() - _GetGameState()->GetFoldPlayerNum() - _GetGameState()->GetAllinPlayerNum();
	if (BettablePlayerNum == 1 && IsBettablePlayerBetMinMoney())
	{
		return false;
	}
	else if (BettablePlayerNum == 0)
	{
		return false;
	}

	return Result;
}

bool ATHGameMode_InGame::IsPlayersLeftToBet()
{
	for (int32 i = 0; i < _GetGameState()->PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(_GetGameState()->PlayerArray[i]);

		if (THPlayer->GetCanBet())
		{
			if (!(IsPlayerAboveMinBettingMoney(THPlayer)) || !(IsPlayerBetOnce(THPlayer)))
			{
				return true;
			}
		}
	}

	return false;
}

bool ATHGameMode_InGame::IsPlayerAboveMinBettingMoney(ATHPlayerState_InGame* THPlayer)
{
	return THPlayer->GetPlayerBetting() == _GetGameState()->GetMinBettingMoney();
}

bool ATHGameMode_InGame::IsPlayerBetOnce(ATHPlayerState_InGame* THPlayer)
{
	return THPlayer->GetBetOnce();
}

bool ATHGameMode_InGame::IsBetAllPlayers()
{
	for (auto Player : _GetGameState()->PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		if (THPlayer->GetCanGameplay() && !THPlayer->GetIsFold() && THPlayer->GetPlayerBetting() == 0)
		{
			return false;
		}
	}

	return true;
}

bool ATHGameMode_InGame::IsBettablePlayerBetMinMoney()
{
	for (auto Player : _GetGameState()->PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		if (THPlayer->GetCanGameplay() && !THPlayer->GetIsFold() && THPlayer->GetBankroll() != 0)
		{
			if (THPlayer->GetPlayerBetting() == _GetGameState()->GetMinBettingMoney())
			{
				return true;
			}
		}
	}

	return false;
}

void ATHGameMode_InGame::SetPlayersWithBestCard(TArray<ATHPlayerState_InGame*> THPlayerArray)
{
	FPokerHand BestHand;
	TArray<FString> Players;

	for (auto THPlayer : THPlayerArray)
	{
		if (Players.IsEmpty())
		{
			BestHand = THPlayer->GetBestHand();
			Players.Add(THPlayer->GetFName().ToString());

			//_GetGameState()->SetBestCards(THPlayer->GetBestCards());
		}
		else
		{
			int32 Result = _GetGameState()->IsBetterThanFormerHand(BestHand, THPlayer->GetBestHand());

			if (Result == 0)
			{
				Players.Add(THPlayer->GetFName().ToString());
			}
			else if (Result < 0)
			{
				Players.Empty();

				BestHand = THPlayer->GetBestHand();
				Players.Add(THPlayer->GetFName().ToString());

				//_GetGameState()->SetBestCards(THPlayer->GetBestCards());
			}
		}
	}

	_GetGameState()->SetWinnerPlayers(Players);
	_GetGameState()->ShowGameResult(Players);
}

void ATHGameMode_InGame::CalculateTableMoney()
{
	int32 TableMoney = 0;
	TArray<int32> PlayerMoney;
	TArray<ATHPlayerState_InGame*> THPlayerArray;

	for (auto Player : _GetGameState()->PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		PlayerMoney.Add(THPlayer->GetPlayerBetting());
		TableMoney += THPlayer->GetPlayerBetting();

		if (THPlayer->GetIsFold() || !(THPlayer->GetCanGameplay()))
		{
			continue;
		}

		THPlayerArray.Add(THPlayer);
	}

	if (_GetGameState()->GetAllinPlayerNum() > 0)
	{
		DividePots(TableMoney, PlayerMoney);
	}
	else
	{
		TArray<FString> Winners = _GetGameState()->GetWinnerPlayers();

		for (auto Player : THPlayerArray)
		{
			if (Winners.Contains(Player->GetFName().ToString()))
			{
				int32 PlayerBankroll = Player->GetBankroll();
				int32 PlayerPot = TableMoney / Winners.Num();

				Player->SetBankroll(PlayerBankroll + PlayerPot);

				UE_LOG(LogTemp, Log, TEXT("%s plus %d to Bankroll"), *(Player->GetFName().ToString()), PlayerPot);
			}
		}
	}
}

void ATHGameMode_InGame::DividePots(int32 TableMoney, TArray<int32> PlayerMoney)
{	
	while (TableMoney > 0)
	{
		TArray<ATHPlayerState_InGame*> PotablePlayers;

		int32 MinBettingMoney = GetMinBettingMoney(PlayerMoney);
		int32 Multiple = 0;
		for (int32 i = 0; i < PlayerMoney.Num(); i++)
		{
			auto THPlayer = Cast<ATHPlayerState_InGame>(_GetGameState()->PlayerArray[i]);

			PlayerMoney[i] -= MinBettingMoney;

			if(PlayerMoney[i] >= 0)
			{
				TableMoney -= MinBettingMoney;
				Multiple++;
				PotablePlayers.Add(THPlayer);
			}
		}

		TArray<ATHPlayerState_InGame*> PotHavingPlayers = GetBestPlayers(PotablePlayers);
		int32 Pot = MinBettingMoney * Multiple;

		for (auto Player : PotHavingPlayers)
		{
			int32 PlayerBankroll = Player->GetBankroll();
			int32 PlayerPot = Pot / PotHavingPlayers.Num();

			Player->SetBankroll(PlayerBankroll + PlayerPot);

			UE_LOG(LogTemp, Log, TEXT("%s plus %d to Bankroll"), *(Player->GetFName().ToString()), PlayerPot);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("DividePots End"));
}

void ATHGameMode_InGame::DecideWinner()
{
	TArray<ATHPlayerState_InGame*> THPlayerArray;

	for (auto Player : _GetGameState()->PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		if (THPlayer->GetIsFold() || !(THPlayer->GetCanGameplay()))
		{
			continue;
		}

		THPlayerArray.Add(THPlayer);
	}

	for (auto THPlayer : THPlayerArray)
	{
		Evaluate(THPlayer, _GetGameState()->GetCommunityCards());
	}

	SetPlayersWithBestCard(THPlayerArray);
}

void ATHGameMode_InGame::Evaluate(ATHPlayerState_InGame* THPlayer, TArray<UTHCard*> CommunityCards)
{
	FPokerHand BestCard;
	TArray<UTHCard*> PlayerHand;
	PlayerHand += THPlayer->GetPlayerCards();
	PlayerHand += CommunityCards;

	PlayerHand.Sort(UTHCard::Comparator);

	TArray<int32> IndexArray2D;
	IndexArray2D = _GetGameState()->GetArrayCombination(PlayerHand.Num(), 5);

	FString PlayerHandString, PlayerCardString;

	for (auto Card : PlayerHand)
	{
		PlayerHandString += Card->GetCardValue();
	}

	for (auto Card : PlayerHand)
	{
		PlayerCardString += Card->GetCardValue();
	}

	for (int32 i = 0; i < (IndexArray2D.Num() / 5); i++)
	{
		TArray<UTHCard*> PlayerBestCards;
		FString CardValues, CardMarks;
		for (int32 j = 0; j < 5; j++)
		{
			CardValues += PlayerHandString[IndexArray2D[i * 5 + j]];
			CardMarks += PlayerHandString[IndexArray2D[i * 5 + j]];
			PlayerBestCards.Add(PlayerHand[IndexArray2D[i * 5 + j]]);
		}

		FPokerHand PHand = _GetGameState()->EvaluatePlayerCard(CardValues, CardMarks);
		PHand.Kicker = _GetGameState()->EvaluateKickers(PHand, PlayerHandString, PlayerCardString);

		if (BestCard.Hands.IsEmpty())
		{
			BestCard = PHand;
			THPlayer->SetBestHand(BestCard);
			THPlayer->SetBestCardNames(PlayerBestCards);
		}

		int32 Result = _GetGameState()->IsBetterThanFormerHand(PHand, BestCard);

		if (Result > 0)
		{
			BestCard = PHand;
			THPlayer->SetBestHand(BestCard);
			THPlayer->SetBestCardNames(PlayerBestCards);
		}
	}
}

void ATHGameMode_InGame::GoNextRoundStage()
{
	if (GetRoundStage() == RoundStage::None)
	{
		SetRoundStage(RoundStage::PreFlop);
	}
	else if (GetRoundStage() == RoundStage::PreFlop)
	{
		SetRoundStage(RoundStage::Flop);
	}
	else if (GetRoundStage() == RoundStage::Flop)
	{
		SetRoundStage(RoundStage::Turn);
	}
	else if (GetRoundStage() == RoundStage::Turn)
	{
		SetRoundStage(RoundStage::River);
	}
	else if (GetRoundStage() == RoundStage::River)
	{
		SetRoundStage(RoundStage::Showdown);
	}
	else if (GetRoundStage() == RoundStage::Showdown)
	{
		SetRoundStage(RoundStage::Waiting);
	}
	else if (GetRoundStage() == RoundStage::Waiting)
	{
		SetRoundStage(RoundStage::PreFlop);
	}
}

void ATHGameMode_InGame::SetRoundStage(FName NewRound)
{
	if (RoundStage == NewRound)
	{
		return;
	}

	SetPreviousRoundStage(RoundStage);

	RoundStage = NewRound;
}

void ATHGameMode_InGame::SetPreviousRoundStage(FName OldRound)
{
	PreivousRoundStage = OldRound;
}

bool ATHGameMode_InGame::ReadyToStartMatch_Implementation()
{
	if (!Super::ReadyToStartMatch_Implementation())
	{
		return false;
	}

	if (GameState->PlayerArray.Num() == 3)
	{
		for (auto Player : GameState->PlayerArray)
		{
			auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

			if (!(THPlayer->GetReadyToPlay()))
			{
				return false;
			}
		}

		return true;
	}

	return false;
}
