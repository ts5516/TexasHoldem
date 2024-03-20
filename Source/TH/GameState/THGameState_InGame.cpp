// Fill out your copyright notice in the Description page of Project Settings.

#include "THGameState_InGame.h"
#include "TH/PlayerController/THPlayerController_InGame.h"
#include "TH/PlayerState/THPlayerState_InGame.h"
#include "TH/Gamemode/THGameMode_InGame.h"
#include "Net/UnrealNetwork.h"

ATHGameState_InGame::ATHGameState_InGame()
	:Super()
{
	DealerIndex = -1;
}

void ATHGameState_InGame::BeginPlay()
{
	Super::BeginPlay();

	Cards = CreateCards();

}

void ATHGameState_InGame::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ATHGameState_InGame::OnRep_MatchState()
{
	Super::OnRep_MatchState();
}

void ATHGameState_InGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATHGameState_InGame, PlayerNames);
	DOREPLIFETIME(ATHGameState_InGame, Cards);
	//DOREPLIFETIME(ATHGameState_InGame, BestCards);
	DOREPLIFETIME(ATHGameState_InGame, CommunityCards);
	DOREPLIFETIME(ATHGameState_InGame, MinBettingMoney);
	DOREPLIFETIME(ATHGameState_InGame, MinRaiseMoney);
}

bool ATHGameState_InGame::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto Card : Cards)
	{
		WroteSomething |= Channel->ReplicateSubobject(Card, *Bunch, *RepFlags);
	}

	for (auto Card : CommunityCards)
	{
		WroteSomething |= Channel->ReplicateSubobject(Card, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void ATHGameState_InGame::TestCards(TArray<FName> TestList)
{
	int32 Index = Cards.Num() - 1;
	for (auto& CardName : TestList)
	{
		for (int32 i = Index; i >= 0; i--)
		{
			if (CardName.IsEqual(Cards[i]->GetCardName()))
			{
				Cards.Swap(i, Index);
				break;
			}
		}
		Index--;

		if (Index < 0)
		{
			break;
		}
	}
}

void ATHGameState_InGame::TestPots(TArray<int32> Money)
{
	TArray<int32> Pots = EvaluatePots(Money);

	for (int32 i = 0; i< Pots.Num(); i++)
	{
		UE_LOG(LogTemp, Log, TEXT("%d Pot Amount is %d"), i, Pots[i]);
	}
}

void ATHGameState_InGame::TestBankroll()
{
	auto THPlayer1 = Cast<ATHPlayerState_InGame>(PlayerArray[0]);
	auto THPlayer2 = Cast<ATHPlayerState_InGame>(PlayerArray[1]);
	auto THPlayer3 = Cast<ATHPlayerState_InGame>(PlayerArray[2]);

	THPlayer1->SetBankroll(20000);
	THPlayer2->SetBankroll(10000);
	THPlayer3->SetBankroll(5000);
}

void ATHGameState_InGame::InitPlayerActions()
{
	TArray<FName> PlayerActions;
	TArray<int32> PlayerBettings;

	for (auto& Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		if (!THPlayer->GetPlayerAction().IsEqual(TEXT("Fold")))
		{
			THPlayer->SetPlayerAction(TEXT(" "));
		}

		PlayerBettings.Add(THPlayer->GetPlayerBetting());
		PlayerActions.Add(THPlayer->GetPlayerAction());
	}

	BroadcastBettingToClient(PlayerBettings, PlayerActions);
}

void ATHGameState_InGame::InitGameInfo()
{
	MinBettingMoney = 0;
	MinRaiseMoney = 0;

	TArray<UTHCard*> RoundUsedCards;

	for (auto Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		TArray<UTHCard*> PlayerCards = THPlayer->GetPlayerCards();

		for (auto Card : PlayerCards)
		{
			RoundUsedCards.Add(DuplicateObject(Card, this));
		}
	}

	for (auto Card : CommunityCards)
	{
		RoundUsedCards.Add(DuplicateObject(Card, this));
	}

	Cards += RoundUsedCards;

	CommunityCards.Empty();
}

void ATHGameState_InGame::InitRoundPlayer()
{
	TArray<bool> CardVisibles;

	for (auto& Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		THPlayer->InitPlayerCards();
		THPlayer->InitPlayerBetting();
		THPlayer->SetPlayerAction(TEXT(" "));

		THPlayer->SetCanBet(true);
		THPlayer->SetBetMyTurn(false);
		THPlayer->SetBetOnce(false);
		THPlayer->SetIsMyTurn(false);
		THPlayer->SetIsFold(false);


		if (THPlayer->GetBankroll() == 0)
		{
			THPlayer->SetCanGameplay(false);
			CardVisibles.Add(false);
			CardVisibles.Add(false);
		}
		else
		{
			CardVisibles.Add(true);
			CardVisibles.Add(true);
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->Client_InitUI(CardVisibles);
		}
	}
}

void ATHGameState_InGame::BettingBlindMoney()
{
	TArray<int32> PlayerBettings;
	TArray<FName> PlayerActions;

	for (auto& Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		int32 BettingMoney = 0;

		if (THPlayer->GetPlayerRole().IsEqual(PokerRole::SmallBlind))
		{
			BettingMoney = GetBlindMoney();
			THPlayer->PlayerBetting(GetBlindMoney());
			THPlayer->SetBetOnce(true);
		}
		else if (THPlayer->GetPlayerRole().IsEqual(PokerRole::BigBlind))
		{
			BettingMoney = GetBlindMoney() * 2;
			THPlayer->PlayerBetting(BettingMoney);
			THPlayer->SetBetOnce(true);
			SetMinBettingMoney(BettingMoney);
			SetMinRaiseMoney(BettingMoney * 2);
		}

		PlayerBettings.Add(THPlayer->GetPlayerBetting());
		PlayerActions.Add(THPlayer->GetPlayerAction());
	}

	BroadcastBettingToClient(PlayerBettings, PlayerActions);
}

void ATHGameState_InGame::BettingFromPlayer(FName PlayerName, int32 BettingAmount, FName Action)
{
	TArray<int32> PlayerBettings;
	TArray<FName> PlayerActions;

	for (auto& Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		if (THPlayer->GetFName().IsEqual(PlayerName))
		{
			THPlayer->PlayerBetting(BettingAmount);
			THPlayer->SetPlayerAction(Action);
			THPlayer->SetBetMyTurn(true);
			THPlayer->SetBetOnce(true);
		}

		if (GetMinBettingMoney() < THPlayer->GetPlayerBetting())
		{
			SetMinBettingMoney(THPlayer->GetPlayerBetting());
		}

		if (GetMinRaiseMoney() <= BettingAmount)
		{
			SetMinRaiseMoney(BettingAmount * 2);
		}

		PlayerBettings.Add(THPlayer->GetPlayerBetting());
		PlayerActions.Add(THPlayer->GetPlayerAction());
	}

	BroadcastBettingToClient(PlayerBettings, PlayerActions);
}

void ATHGameState_InGame::BroadcastBettingToClient(TArray<int32> PlayerBettings, TArray<FName> PlayerActions)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->Client_ShowPlayerBetting(PlayerBettings, PlayerActions);
		}
	}
}

TArray<UTHCard*> ATHGameState_InGame::CreateCards()
{
	TArray<UTHCard*> _Cards;

	for (auto& CardName : CardNames)
	{
		UTHCard* NewCard = NewObject<UTHCard>(this);
		NewCard->SetCardName(CardName);
		_Cards.Add(NewCard);
	}

	return _Cards;
}

void ATHGameState_InGame::ShuffleCards()
{
	if (Cards.Num() > 0)
	{
		int32 LastIndex = Cards.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Cards.Swap(i, Index);
			}
		}
	}
}

void ATHGameState_InGame::DivideCardToPlayer()
{
	for (auto& Player : PlayerArray)
	{	
		int32 LastIndex = Cards.Num() - 1;

		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		THPlayer->SetPlayerCards(Cards[LastIndex], Cards[LastIndex - 1]);

		Cards.RemoveAt(LastIndex);
		Cards.RemoveAt(LastIndex - 1);
	}
}

void ATHGameState_InGame::OpenFlopCards()
{
	int32 LastIndex = Cards.Num() - 1;

	CommunityCards.Add(DuplicateObject(Cards[LastIndex], this));
	CommunityCards.Add(DuplicateObject(Cards[LastIndex - 1], this));
	CommunityCards.Add(DuplicateObject(Cards[LastIndex - 2], this));

	Cards.RemoveAt(LastIndex);
	Cards.RemoveAt(LastIndex - 1);
	Cards.RemoveAt(LastIndex - 2);
}

void ATHGameState_InGame::OpenCard()
{
	int32 LastIndex = Cards.Num() - 1;

	CommunityCards.Add(DuplicateObject(Cards[LastIndex], this));

	Cards.RemoveAt(LastIndex);
}

void ATHGameState_InGame::ShowPlayerCards()
{
	TArray<UTHCard*> AllPlayerCards;
	TArray<bool> IsPutFrontCards;

	for (auto Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		TArray<UTHCard*> PlayerCards = THPlayer->GetPlayerCards();

		if (THPlayer->GetIsFold())
		{
			IsPutFrontCards.Add(false);
			IsPutFrontCards.Add(false);
		}
		else
		{
			IsPutFrontCards.Add(true);
			IsPutFrontCards.Add(true);
		}
		
		AllPlayerCards += PlayerCards;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->Client_ShowPlayerCards(AllPlayerCards, IsPutFrontCards);
		}
	}
}

void ATHGameState_InGame::ShowGameResult(TArray<FString> _WinnerPlayers)
{
	TArray<FName> BestCardNames;

	for (auto Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		TArray<FName> PlayerBestCards = THPlayer->GetBestCardNames();
		FString PlayerName = THPlayer->GetFName().ToString();
		
		if (_WinnerPlayers.Contains(PlayerName))
		{
			BestCardNames += PlayerBestCards;
			break;
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			int32 GameResult = 1;
			FString PlayerName = PlayerController->GetPlayerState()->GetFName().ToString();
			if (!_WinnerPlayers.Contains(PlayerName))
			{
				GameResult = -1;
			}
			else if (_WinnerPlayers.Num() > 1)
			{
				GameResult = 0;
			}

			PlayerController->Client_ShowGameResult(BestCardNames, GameResult);
		}
	}
}

void ATHGameState_InGame::ShowGameEndMessage()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->Client_ShowGameEndMessage();
		}
	}
}

//Cards must be sorted
bool ATHGameState_InGame::IsStraight(FString CardValues)
{
	FString PokerString = "AKQJ0987654321";
	
	if (CardValues.Contains(TEXT("A")))
	{
		return (CardValues.Equals(TEXT("A5432")) || CardValues.Equals(TEXT("AKQJ0")));
	}

	return (PokerString.Find(CardValues) > 0);

}

bool ATHGameState_InGame::IsFlush(FString CardMarks)
{
	return CardMarks.Equals(TEXT("HHHHH"))
		|| CardMarks.Equals(TEXT("CCCCC"))
		|| CardMarks.Equals(TEXT("SSSSS"))
		|| CardMarks.Equals(TEXT("DDDDD"));
}

TArray<int32> ATHGameState_InGame::EvaluatePots(TArray<int32> Money)
{
	TArray<int32> Pots;

	Money.Sort();

	for (int32 i = 0; i < Money.Num(); i++)
	{
		if (Money[i] == 0)
		{
			continue;
		}

		int Cash = Money[i];
		for (int32 j = i; j < Money.Num(); j++)
		{
			Money[j] -= Cash;
		}

		Pots.Add(Cash * (Money.Num() - i));
	}

	return Pots;
}

FPokerHand ATHGameState_InGame::EvaluatePlayerCard(FString CardValues, FString CardMarks)
{
	FPokerHand PHand;
	PHand.Hands = CardValues;

	if (IsStraight(CardValues) && IsFlush(CardMarks))
	{
		PHand.Rank = PokerRank::StraightFlush;
		PHand.Highs += CardValues[0]; // if this value is A, RSF
	}
	else if (CardValues[0] == CardValues[3] || CardValues[1] == CardValues[4])
	{
		PHand.Rank = PokerRank::FourOfaKind;
		PHand.Highs += CardValues[2];
	}
	else if (CardValues[0] == CardValues[2] && CardValues[3] == CardValues[4]
		|| CardValues[2] == CardValues[4] && CardValues[0] == CardValues[1])
	{
		PHand.Rank = PokerRank::FullHouse;
		PHand.Highs += CardValues[2];
	}
	else if (IsFlush(CardMarks))
	{
		PHand.Rank = PokerRank::Flush;
		PHand.Highs += CardValues[0];
	}
	else if (IsStraight(CardValues))
	{
		PHand.Rank = PokerRank::Straight;
		PHand.Highs += CardValues[0];
	}
	else if (CardValues[0] == CardValues[2]
		|| CardValues[1] == CardValues[3]
		|| CardValues[2] == CardValues[4])
	{
		PHand.Rank = PokerRank::ThreeOfaKind;
		PHand.Highs += CardValues[2];
	}
	else if (CardValues[0] == CardValues[1] && CardValues[2] == CardValues[3]
		|| CardValues[0] == CardValues[1] && CardValues[3] == CardValues[4]
		|| CardValues[1] == CardValues[2] && CardValues[3] == CardValues[4]) // Two Pairs
	{
		PHand.Rank = PokerRank::TwoPairs;
		PHand.Highs += CardValues[1];
		PHand.Highs += CardValues[3];
	}
	else if (CardValues[0] == CardValues[1]
		|| CardValues[1] == CardValues[2]
		|| CardValues[2] == CardValues[3]
		|| CardValues[3] == CardValues[4]) // One Pairs
	{
		PHand.Rank = PokerRank::OnePairs;

		if (CardValues[0] == CardValues[1])
		{
			PHand.Highs += CardValues[0];
		}
		else if(CardValues[1] == CardValues[2])
		{
			PHand.Highs += CardValues[1];
		}
		else if (CardValues[2] == CardValues[3])
		{
			PHand.Highs += CardValues[2];
		}
		else
		{
			PHand.Highs += CardValues[3];
		}
	}
	else  // High Card
	{
		PHand.Rank = PokerRank::HighCard;
	}

	return PHand;
}

FString ATHGameState_InGame::EvaluateKickers(FPokerHand PHand, FString TotalCards, FString PlayerHands)
{
	if (PHand.Rank == PokerRank::StraightFlush
		|| PHand.Rank == PokerRank::FullHouse
		|| PHand.Rank == PokerRank::Flush
		|| PHand.Rank == PokerRank::Straight)
	{
		return TEXT("");
	}

	int32 BeforeTotalCardsLen = TotalCards.Len();
	for (int32 i = 0; i < PHand.Highs.Len(); i++)
	{
		auto PHigh = PHand.Highs[i];
		TotalCards = TotalCards.Replace(&PHigh, TEXT(""));
		PlayerHands = PlayerHands.Replace(&PHigh, TEXT(""));
	}

	int32 AfterTotalCardsLen = TotalCards.Len();

	int32 Gap = BeforeTotalCardsLen - AfterTotalCardsLen;
	FString Kickers;
	for (int32 i = 0; i < 5 - Gap; i++)
	{
		Kickers += TotalCards[i];
	}

	return Kickers;
}

int32 ATHGameState_InGame::IsBetterThanFormerHand(FPokerHand PlayerHand1, FPokerHand PlayerHand2)
{
	// Compare Rank
	if (PlayerHand1.Rank - PlayerHand2.Rank != 0)
	{
		return PlayerHand1.Rank - PlayerHand2.Rank;
	}

	// Compare High
	TArray<int32> PlayerHigh1 = PlayerHand1.ConvertToIntArray(PlayerHand1.Highs);
	TArray<int32> PlayerHigh2 = PlayerHand2.ConvertToIntArray(PlayerHand2.Highs);

	for (int32 i = 0; i < PlayerHigh1.Num(); i++)
	{
		if (PlayerHigh1[i] != PlayerHigh2[i])
		{
			return PlayerHigh1[i] - PlayerHigh2[i];
		}
	}

	// Compare Kicker
	TArray<int32> PlayerKicker1 = PlayerHand1.ConvertToIntArray(PlayerHand1.Kicker);
	TArray<int32> PlayerKicker2 = PlayerHand2.ConvertToIntArray(PlayerHand2.Kicker);

	for (int32 i = 0; i < PlayerKicker1.Num(); i++)
	{
		if (PlayerKicker1[i] != PlayerKicker2[i])
		{
			return PlayerKicker1[i] - PlayerKicker2[i];
		}
	}

	return 0;
}

TArray<int32> ATHGameState_InGame::GetArrayCombination(int32 TotalNum, int32 ChooseNum)
{
	TArray<int32> CombArray;
	TArray<int32> Comb;

	Comb.Init(0, ChooseNum);
	
	Combination(CombArray, TotalNum, Comb, ChooseNum, 0, 0);

	return CombArray;
}

void ATHGameState_InGame::Combination(TArray<int32>& CombArray, int32 TotalNum, TArray<int32> Comb, int32 R, int32 Index, int32 Depth)
{
	if (R == 0)
	{
		CombArray.Append(Comb);
	}
	else if (Depth == TotalNum)  // 계속 안뽑다가 r 개를 채우지 못한 경우는 이 곳에 걸려야 한다.
	{
		return;
	}
	else
	{
		// Depth를 뽑은 경우
		Comb[Index] = Depth;
		Combination(CombArray, TotalNum, Comb, R - 1, Index + 1, Depth + 1);

		// Depth를 뽑지 않은 경우
		Combination(CombArray, TotalNum, Comb, R, Index, Depth + 1);
	}
}

void ATHGameState_InGame::OnRep_PlayerNames()
{
	/*if (PlayerNames.Num() != PlayerArray.Num())
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->SetbUpdatePlayerNames(true);
		}
	}*/
}

void ATHGameState_InGame::OnRep_CommunityCards()
{
	if (CommunityCards.Num() >= 3)
	{
		for (auto Card : CommunityCards)
		{
			if (!Card)
			{
				return;
			}
		}

		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
			if (PlayerController)
			{
				PlayerController->SetUpdateCommunityCards(true);
			}
		}
	}
}

void ATHGameState_InGame::RoundTimerStart()
{
	RoundEndTime = ElapsedTime;
}

TArray<FName> ATHGameState_InGame::GetPlayerRoles()
{
	TArray<FName> PlayerRoles;

	for (int32 Index = 0; Index < PlayerArray.Num(); Index++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[Index]);

		if (DealerIndex == Index && THPlayer->GetPlayerRole().IsEqual(PokerRole::SmallBlind))
		{
			FString DAndSB = PokerRole::Dealer.ToString() + ", " + PokerRole::SmallBlind.ToString();
			PlayerRoles.Add(FName(*DAndSB));
		}
		else
		{
			PlayerRoles.Add(THPlayer->GetPlayerRole());
		}
	}

	return PlayerRoles;
}

void ATHGameState_InGame::SetPlayerRoles()
{
	DealerIndex = GetNextGameablePlayerIndex(DealerIndex);

	InitPlayerRoles();

	if (GetGameablePlayerNum() > 2)
	{
		auto DealerPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[DealerIndex]);
		DealerPlayer->SetPlayerRole(PokerRole::Dealer);

		int32 SmallBlindIndex = GetNextGameablePlayerIndex(DealerIndex);
		auto SmallBlindPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[SmallBlindIndex]);
		SmallBlindPlayer->SetPlayerRole(PokerRole::SmallBlind);

		int32 BigBlindIndex = GetNextGameablePlayerIndex(DealerIndex + 1);
		auto BigBlindPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[BigBlindIndex]);
		BigBlindPlayer->SetPlayerRole(PokerRole::BigBlind);
	}
	else
	{
		auto DealerPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[DealerIndex]);
		DealerPlayer->SetPlayerRole(PokerRole::SmallBlind);

		int32 BigBlindIndex = GetNextGameablePlayerIndex(DealerIndex);
		auto BigBlindPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[BigBlindIndex]);
		BigBlindPlayer->SetPlayerRole(PokerRole::BigBlind);
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ATHPlayerController_InGame* PlayerController = Cast<ATHPlayerController_InGame>(Iterator->Get());
		if (PlayerController)
		{
			PlayerController->Client_ShowPlayerInfo(GetPlayerRoles());
		}
	}
}

void ATHGameState_InGame::InitPlayerRoles()
{
	for (auto Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);
		THPlayer->SetPlayerRole(PokerRole::None);
	}
}

int32 ATHGameState_InGame::GetPlayerIndexFromRole(FName _PokerRole)
{
	for (int32 Index = 0; Index < PlayerArray.Num(); Index++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[Index]);
		
		if (THPlayer->GetPlayerRole() == _PokerRole)
		{
			return Index;
		}
	}

	return -1;
}

int32 ATHGameState_InGame::GetFoldPlayerNum()
{
	int32 FoldPlayerNum = 0;

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[i]);

		if (THPlayer->GetIsFold())
		{
			FoldPlayerNum++;
		}
	}

	return FoldPlayerNum;
}

int32 ATHGameState_InGame::GetGameablePlayerNum()
{
	int32 GameablePlayerNum = 0;

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[i]);

		if (THPlayer->GetCanGameplay())
		{
			GameablePlayerNum++;
		}
	}

	return GameablePlayerNum;
}

int32 ATHGameState_InGame::GetAllinPlayerNum()
{
	int32 AllinPlayerNum = 0;

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[i]);

		if (THPlayer->GetBankroll() == 0 && THPlayer->GetCanGameplay())
		{
			AllinPlayerNum++;
		}
	}

	return AllinPlayerNum;
}

int32 ATHGameState_InGame::GetNextGameablePlayerIndex(int32 Index)
{
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		Index = (Index + 1) % PlayerArray.Num();
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[Index]);

		if (THPlayer->GetCanGameplay())
		{
			return Index;
		}
	}

	return -1;
}

int32 ATHGameState_InGame::GetNextBettingPlayerIndex()
{
	int32 NextPlayer = GetNextGameablePlayerIndex(TurnPlayer);

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[NextPlayer]);

		if (THPlayer->GetCanBet())
		{
			return NextPlayer;
		}

		NextPlayer = GetNextGameablePlayerIndex(NextPlayer);
	}

	return NextPlayer;
}

bool ATHGameState_InGame::IsEndedWaiting()
{
	if (ElapsedTime - RoundEndTime > RoundInitTime)
	{
		return true;
	}
	
	return false;
}

//void ATHGameState_InGame::SetBestCards(TArray<UTHCard*> PlayerBestCards)
//{
//	if (!(BestCards.IsEmpty()))
//	{
//		BestCards.Empty();
//	}
//
//	for (auto Card : PlayerBestCards)
//	{
//		BestCards.Add(DuplicateObject(Card, this));
//	}
//}

void ATHGameState_InGame::SetTurnPlayerIndex(FName _RoundStage)
{
	if (_RoundStage.IsEqual(RoundStage::PreFlop))
	{
		TurnPlayer = GetNextGameablePlayerIndex(GetPlayerIndexFromRole(PokerRole::BigBlind));
	}
	else
	{
		TurnPlayer = GetPlayerIndexFromRole(PokerRole::SmallBlind);
	}

	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(PlayerArray[TurnPlayer]);

		if (THPlayer->GetCanBet())
		{
			return;
		}

		TurnPlayer = (TurnPlayer + 1) % (PlayerArray.Num());
	}
}

void ATHGameState_InGame::SetNextTurnPlayerIndex()
{
	TurnPlayer = GetNextBettingPlayerIndex();

	auto Player = Cast<ATHPlayerState_InGame>(PlayerArray[TurnPlayer]);
	Player->SetIsMyTurn(true);
}

void ATHGameState_InGame::SetTurnPlayer(FName _RoundStage)
{
	SetTurnPlayerIndex(_RoundStage);

	auto Player = Cast<ATHPlayerState_InGame>(PlayerArray[TurnPlayer]);
	Player->SetIsMyTurn(true);
}

void ATHGameState_InGame::InitTurnPlayer()
{
	for (auto& Player : PlayerArray)
	{
		auto THPlayer = Cast<ATHPlayerState_InGame>(Player);

		THPlayer->SetIsMyTurn(false);
		THPlayer->SetBetMyTurn(false);
		THPlayer->SetBetOnce(false);
	}
}
