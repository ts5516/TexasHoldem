// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THGameMode.h"
#include "TH/PlayerController/THPlayerController_InGame.h"
#include "THGameMode_InGame.generated.h"

namespace RoundStage
{
	const FName None = FName(TEXT("None"));
	const FName PreFlop = FName(TEXT("PreFlop"));
	const FName Flop = FName(TEXT("Flop"));
	const FName Turn = FName(TEXT("Turn"));
	const FName River = FName(TEXT("River"));
	const FName Showdown = FName(TEXT("Showdown"));
	const FName Waiting = FName(TEXT("Waiting"));
}


/**
 * 
 */
UCLASS()
class TH_API ATHGameMode_InGame : public ATHGameMode
{
	GENERATED_BODY()

public:
	ATHGameMode_InGame();
	
	virtual void InitGameState() override;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleMatchHasStarted() override;
	virtual void Tick(float DeltaSeconds) override;

	ATHGameState_InGame* _GetGameState() const { return GetGameState<ATHGameState_InGame>(); }

	void GoNextRoundStage();
	void ReadyToBetting();
	void StartBetting();

	void ProcessByRoundStage();

	bool IsValidBettingSituation();
	bool IsPlayersLeftToBet();
	bool IsPlayerAboveMinBettingMoney(ATHPlayerState_InGame* THPlayer);
	bool IsPlayerBetOnce(ATHPlayerState_InGame* THPlayer);
	bool IsBetAllPlayers();
	bool IsBettablePlayerBetMinMoney();

	void SetPlayersWithBestCard(TArray<ATHPlayerState_InGame*> THPlayerArray);
	void CalculateTableMoney();
	void DividePots(int32 TableMoney, TArray<int32> PlayerMoney);
	void DecideWinner();

	void Evaluate(ATHPlayerState_InGame* THPlayer, TArray<UTHCard*> CommunityCard);

	FName GetRoundStage() { return RoundStage; }
	void SetRoundStage(FName NewRound);

	FName GetPreviousRoundStage() { return PreivousRoundStage; }
	void SetPreviousRoundStage(FName OldRound);

	void InitGameRound();

	bool GetbIsThePlayerBetting() { return bIsThePlayerBetting; }
	void SetbIsThePlayerBetting(bool _bIsThePlayerBetting) { bIsThePlayerBetting = _bIsThePlayerBetting; }
	
	bool GetbReadyToBetting() { return bReadyToBetting; }
	void SetbReadyToBetting(bool _bReadyToBetting) { bReadyToBetting = _bReadyToBetting; }

	int32 GetMinBettingMoney(TArray<int32> PlayerMoney);
	TArray<ATHPlayerState_InGame*> GetBestPlayers(TArray<ATHPlayerState_InGame*> Players);
private:

	FName RoundStage;
	FName PreivousRoundStage;

	bool bIsThePlayerBetting = false;
	bool bReadyToBetting = false;

	int32 RoundCount = 0;
};
