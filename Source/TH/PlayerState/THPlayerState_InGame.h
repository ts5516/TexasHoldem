// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THPlayerState.h"
#include "THPlayerState_InGame.generated.h"

class UTHCard;
struct FPokerHand;

/**
 * 
 */
UCLASS()
class TH_API ATHPlayerState_InGame : public ATHPlayerState
{
	GENERATED_BODY()

public:

	ATHPlayerState_InGame();

	void PlayerBetting(int32 Amount);
	void InitPlayerBetting();


	void SetPlayerCards(UTHCard* Card1, UTHCard* Card2);
	void SetBestCards(TArray<UTHCard*> _BestCards);
	void SetBestCardNames(TArray<UTHCard*> _BestCards);
	void SetIsMyTurn(bool _bIsMyTurn);
	void SetBetMyTurn(bool _bBetMyTurn);
	void SetReadyToPlay(bool _bReadyToPlay) { bReadyToPlay = _bReadyToPlay; }
	void SetPlayerRole(FName _PlayerRole);
	void SetBankroll(int32 Amount) { Bankroll = Amount; }
	void SetBetOnce(bool _bBetOnce) { bBetOnce = _bBetOnce; }
	void SetPlayerAction(FName NewAction) { PlayerAction = NewAction; }
	void SetCanBet(bool _bCanBet) { bCanBet = _bCanBet; }
	void SetBestHand(FPokerHand _BestHand) { BestHand = _BestHand; }
	void SetIsFold(bool _bIsFold) { bIsFold = _bIsFold; }
	void SetCanGameplay(bool _bCanGameplay) { bCanGameplay = _bCanGameplay; }


	bool GetIsFold() { return bIsFold; }
	bool GetCanBet() { return bCanBet; }
	bool GetIsMyTurn() { return bIsMyTurn; }
	bool GetBetMyTurn() { return bBetMyTurn; }
	bool GetReadyToPlay() { return bReadyToPlay; }
	bool GetBetOnce() { return bBetOnce; }
	bool GetCanGameplay() { return bCanGameplay; }
	FName GetPlayerRole() { return PlayerRole; }
	FName GetPlayerAction() { return PlayerAction; }
	TArray<UTHCard*> GetPlayerCards() { return PlayerCards; }
	TArray<UTHCard*> GetBestCards() { return BestCards; }
	TArray<FName> GetBestCardNames() { return BestCardNames; }
	FPokerHand GetBestHand() { return BestHand; }
	
	void InitPlayerCards();

	UFUNCTION(BlueprintCallable)
	int32 GetBankroll() { return Bankroll; }

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerBetting() { return Betting; }

	UFUNCTION()
	void OnRep_PlayerCards();

	UFUNCTION()
	void OnRep_bIsMyTurn();

	UFUNCTION()
	void OnRep_bBetMyTurn();

	UFUNCTION()
	void OnRep_PlayerRole();

	UFUNCTION()
	void OnRep_Bankroll();

	UFUNCTION()
	void OnRep_CanGameplay();

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	bool bBetOnce = false;

	UPROPERTY(ReplicatedUsing = OnRep_bIsMyTurn)
	bool bIsMyTurn = false;

	UPROPERTY(ReplicatedUsing = OnRep_bBetMyTurn)
	bool bBetMyTurn = false;

	UPROPERTY(Replicated)
	bool bReadyToPlay = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_CanGameplay)
	bool bCanGameplay = true;

	UPROPERTY(Replicated)
	bool bCanBet = true;

	UPROPERTY(Replicated)
	bool bIsFold = false;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerCards)
	TArray<UTHCard*> PlayerCards;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerRole)
	FName PlayerRole;

	UPROPERTY(Replicated)
	FName PlayerAction;

	UPROPERTY(ReplicatedUsing = OnRep_Bankroll)
	int32 Bankroll = 10000;

	UPROPERTY(Replicated)
	int32 Betting;

	UPROPERTY(Replicated)
	FPokerHand BestHand;

	UPROPERTY(Replicated)
	TArray<UTHCard*> BestCards;

	UPROPERTY(Replicated)
	TArray<FName> BestCardNames;
};
