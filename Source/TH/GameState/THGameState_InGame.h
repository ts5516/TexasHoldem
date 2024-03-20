// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THGameState.h"
#include "TH/GameInfo/THRoom.h"
#include "TH/GameInfo/THCard.h"
#include "THGameState_InGame.generated.h"

namespace PokerRole
{
	const FName None = FName(TEXT("N"));
	const FName Dealer = FName(TEXT("D"));
	const FName BigBlind = FName(TEXT("BB"));
	const FName SmallBlind = FName(TEXT("SB"));
}

enum PokerRank
{
	HighCard = 0,
	OnePairs = 1,
	TwoPairs,
	ThreeOfaKind,
	Straight,
	Flush,
	FullHouse,
	FourOfaKind,
	StraightFlush
};

USTRUCT()
struct FPokerHand
{
	GENERATED_BODY()

	UPROPERTY()
	FString Hands;

	UPROPERTY()
	int32 Rank;

	UPROPERTY()
	FString Highs;

	UPROPERTY()
	FString Kicker;

	TArray<int32> ConvertToIntArray(FString CardNumbers)
	{
		FString PokerNumbers = "234567890JQKA";
		TArray<int32> CardValues;

		for (int32 i = 0; i < PokerNumbers.Len(); i++)
		{
			FString PNum;
			PNum += PokerNumbers[i];
			if (CardNumbers.Contains(PNum))
			{
				CardValues.Add(i);
			}
		}

		return CardValues;
	}
};

/**
 * 
 */
UCLASS()
class TH_API ATHGameState_InGame : public ATHGameState
{
	GENERATED_BODY()

public:

	ATHGameState_InGame();

	virtual void BeginPlay() override;
	virtual void HandleMatchHasStarted() override;
	virtual void OnRep_MatchState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:

	void TestCards(TArray<FName> TestList);
	void TestPots(TArray<int32> Money);
	void TestBankroll();

	void InitPlayerActions();
	void InitGameInfo();
	void InitRoundPlayer();

	void BettingBlindMoney();
	void BettingFromPlayer(FName PlayerName, int32 BettingAmount, FName Action);
	void BroadcastBettingToClient(TArray<int32> PlayerBettings, TArray<FName> PlayerActions);


	TArray<UTHCard*> CreateCards();
	void ShuffleCards();
	void DivideCardToPlayer();
	void OpenFlopCards();
	void OpenCard();
	void ShowPlayerCards();
	void ShowGameResult(TArray<FString> Players);
	void ShowGameEndMessage();
	
	TArray<int32> GetArrayCombination(int32 CardValues, int32 ChooseNum);
	void Combination(TArray<int32>& CombArray, int32 CardValues, TArray<int32> Comb, int32 R, int32 Index, int32 Depth);

	bool IsStraight(FString CardValues);
	bool IsFlush(FString CardMarks);

	TArray<int32> EvaluatePots(TArray<int32> Money);

	FPokerHand EvaluatePlayerCard(FString CardValues, FString CardMarks);
	FString EvaluateKickers(FPokerHand PHand, FString TotalCards, FString PlayerHands);
	int32 IsBetterThanFormerHand(FPokerHand PlayerHand1, FPokerHand PlayerHand2);

	void AddPlayerName(FName PlayerName) { PlayerNames.Add(PlayerName); }

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetPlayerName() { return PlayerNames; }

	UFUNCTION()
	void OnRep_PlayerNames();

	UFUNCTION()
	void OnRep_CommunityCards();

	void RoundTimerStart();
	int32 RoundEndTime;
	int32 RoundInitTime;

	bool IsEndedWaiting();

	bool GetbIsBettingStage() { return bIsBettingStage; }
	int32 GetDealerIndex() { return DealerIndex; }
	TArray<UTHCard*> GetCommunityCards() { return CommunityCards; }
	TArray<FName> GetPlayerRoles();
	//TArray<UTHCard*> GetBestCards() { return BestCards; }
	TArray<FString> GetWinnerPlayers() { return WinnerPlayers; }
	
	int32 GetPlayerIndexFromRole(FName _PokerRole);
	int32 GetFoldPlayerNum();
	int32 GetGameablePlayerNum();
	int32 GetAllinPlayerNum();
	int32 GetNextGameablePlayerIndex(int32 Index);
	int32 GetNextBettingPlayerIndex();
	int32 GetTurnPlayerIndex() { return TurnPlayer; }

	UFUNCTION(BlueprintCallable)
	int32 GetMinBettingMoney() { return MinBettingMoney; }

	UFUNCTION(BlueprintCallable)
	int32 GetMinRaiseMoney() { return MinRaiseMoney; }

	int32 GetBlindMoney() { return BlindMoney; }

	//void SetBestCards(TArray<UTHCard*> PlayerBestCards);
	void SetWinnerPlayers(TArray<FString> _WinnerPlayers) { WinnerPlayers = _WinnerPlayers; }
	void SetMinBettingMoney(int32 Amount) { MinBettingMoney = Amount; }
	void SetMinRaiseMoney(int32 Amount) { MinRaiseMoney = Amount; }
	void SetTurnPlayerIndex(FName _RoundStage);
	void SetNextTurnPlayerIndex();

	void SetTurnPlayer(FName _RoundStage);
	void InitTurnPlayer();

	void SetPlayerRoles();
	void InitPlayerRoles();

private:
	
	TArray<FString> WinnerPlayers;

	//UPROPERTY(ReplicatedUsing = OnRep_BestCards)
	//TArray<UTHCard*> BestCards;

	int32 TurnPlayer = -1;

	bool bIsBettingStage = false;

	UPROPERTY(Replicated)
	int32 MinBettingMoney = 0;

	UPROPERTY(Replicated)
	int32 MinRaiseMoney = 0;

	int32 BlindMoney = 100;

	int32 DealerIndex;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerNames)
	TArray<FName> PlayerNames;

	UPROPERTY(Replicated)
	TArray<UTHCard*> Cards;

	UPROPERTY(ReplicatedUsing = OnRep_CommunityCards)
	TArray<UTHCard*> CommunityCards;

	TArray<FName> CardNames =
	{
		"ClubA","Club2","Club3","Club4","Club5","Club6","Club7","Club8","Club9","Club10","ClubJ","ClubQ","ClubK",
		"DiaA","Dia2","Dia3","Dia4","Dia5","Dia6","Dia7","Dia8","Dia9","Dia10","DiaJ","DiaQ","DiaK",
		"HeartA","Heart2","Heart3","Heart4","Heart5","Heart6","Heart7","Heart8","Heart9","Heart10","HeartJ","HeartQ","HeartK",
		"SpadeA","Spade2","Spade3","Spade4","Spade5","Spade6","Spade7","Spade8","Spade9","Spade10","SpadeJ","SpadeQ","SpadeK",
	};
};
