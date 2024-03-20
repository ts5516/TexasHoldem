// Fill out your copyright notice in the Description page of Project Settings.


#include "THPlayerState_InGame.h"
#include "TH/GameInfo/THCard.h"
#include "Engine/ActorChannel.h"
#include "TH/PlayerController/THPlayerController_InGame.h"
#include "TH/GameState/THGameState_InGame.h"
#include "Net/UnrealNetwork.h"


ATHPlayerState_InGame::ATHPlayerState_InGame()
{
	bAlwaysRelevant = true;
	bReplicates = true;

	PlayerRole = PokerRole::None;
}

void ATHPlayerState_InGame::PlayerBetting(int32 Amount)
{
	if (Bankroll < Amount)
	{
		return;
	}

	Bankroll -= Amount;
	Betting += Amount;
}

void ATHPlayerState_InGame::InitPlayerBetting()
{
	Betting = 0;
}

void ATHPlayerState_InGame::SetPlayerCards(UTHCard* Card1, UTHCard* Card2)
{
	PlayerCards.Add(DuplicateObject(Card1, this));
	PlayerCards.Add(DuplicateObject(Card2, this));
}

void ATHPlayerState_InGame::SetBestCards(TArray<UTHCard*> _BestCards)
{
	if (!(BestCards.IsEmpty()))
	{
		BestCards.Empty();
	}

	for (auto Card : _BestCards)
	{
		BestCards.Add(DuplicateObject(Card, this));
	}
}

void ATHPlayerState_InGame::SetBestCardNames(TArray<UTHCard*> _BestCards)
{
	BestCardNames.Empty();

	for (auto Card : _BestCards)
	{
		BestCardNames.Add(Card->GetCardName());
	}
}

void ATHPlayerState_InGame::SetIsMyTurn(bool _bIsMyTurn)
{
	if (bIsMyTurn == _bIsMyTurn)
	{
		return;
	}

	bIsMyTurn = _bIsMyTurn;
}

void ATHPlayerState_InGame::SetBetMyTurn(bool _bBetMyTurn)
{
	if (bBetMyTurn == _bBetMyTurn)
	{
		return;
	}

	bBetMyTurn = _bBetMyTurn;
}

void ATHPlayerState_InGame::SetPlayerRole(FName _PlayerRole)
{
	if (PlayerRole == _PlayerRole)
	{
		return;
	}

	PlayerRole = _PlayerRole;
}

void ATHPlayerState_InGame::InitPlayerCards()
{
	if (PlayerCards.IsEmpty())
	{
		return;
	}

	PlayerCards.Empty();
}

void ATHPlayerState_InGame::OnRep_PlayerCards()
{
	if (PlayerCards.Num() < 2)
	{
		return;
	}

	for (auto Card : PlayerCards)
	{
		if (!Card)
		{
			return;
		}
	}

	if (ATHPlayerController_InGame* PC = Cast<ATHPlayerController_InGame>(GetPlayerController()))
	{
		PC->SetUpdateWidget(true);
	}
}

void ATHPlayerState_InGame::OnRep_bIsMyTurn()
{
	if (ATHPlayerController_InGame* PC = Cast<ATHPlayerController_InGame>(GetPlayerController()))
	{
		PC->SetUpdateHUD(true);
	}
}

void ATHPlayerState_InGame::OnRep_bBetMyTurn()
{

}

void ATHPlayerState_InGame::OnRep_PlayerRole()
{
	/*if (ATHPlayerController_InGame* PC = Cast<ATHPlayerController_InGame>(GetPlayerController()))
	{
		PC->SetUpdatePlayerRole(true);
	}*/
}

void ATHPlayerState_InGame::OnRep_Bankroll()
{
	if (ATHPlayerController_InGame* PC = Cast<ATHPlayerController_InGame>(GetPlayerController()))
	{
		PC->SetUpdateBankroll(true);
	}
}

void ATHPlayerState_InGame::OnRep_CanGameplay()
{
	if (ATHPlayerController_InGame* PC = Cast<ATHPlayerController_InGame>(GetPlayerController()))
	{
		PC->InitHUD(bCanGameplay);
	}
}

bool ATHPlayerState_InGame::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto PlayerCard : PlayerCards)
	{
		WroteSomething |= Channel->ReplicateSubobject(PlayerCard, *Bunch, *RepFlags);
	}

	for (auto Card : BestCards)
	{
		WroteSomething |= Channel->ReplicateSubobject(Card, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void ATHPlayerState_InGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATHPlayerState_InGame, PlayerCards);
	DOREPLIFETIME(ATHPlayerState_InGame, BestCardNames);
	DOREPLIFETIME(ATHPlayerState_InGame, BestCards);
	DOREPLIFETIME(ATHPlayerState_InGame, BestHand);
	DOREPLIFETIME(ATHPlayerState_InGame, bIsMyTurn);
	DOREPLIFETIME(ATHPlayerState_InGame, bBetMyTurn);
	DOREPLIFETIME(ATHPlayerState_InGame, bReadyToPlay);
	DOREPLIFETIME(ATHPlayerState_InGame, PlayerRole);
	DOREPLIFETIME(ATHPlayerState_InGame, PlayerAction);
	DOREPLIFETIME(ATHPlayerState_InGame, Bankroll);
	DOREPLIFETIME(ATHPlayerState_InGame, Betting);
	DOREPLIFETIME(ATHPlayerState_InGame, bBetOnce);
	DOREPLIFETIME(ATHPlayerState_InGame, bCanBet);
	DOREPLIFETIME(ATHPlayerState_InGame, bIsFold);
	DOREPLIFETIME(ATHPlayerState_InGame, bCanGameplay);
}
