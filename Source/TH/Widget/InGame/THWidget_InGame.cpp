// Fill out your copyright notice in the Description page of Project Settings.


#include "THWidget_InGame.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TH/GameState/THGameState_InGame.h"


UTHWidget_InGame::UTHWidget_InGame(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTHWidget_InGame::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 1; i <= 3; i++)
	{
		FName Name = FName(*FString::Printf(TEXT("P%d_Name"), i));
		FName Action = FName(*FString::Printf(TEXT("P%d_Action"), i));
		FName Role = FName(*FString::Printf(TEXT("P%d_Role"), i));
		FName Bet = FName(*FString::Printf(TEXT("P%d_Bet"), i));

		UTextBlock* NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(Name));
		UTextBlock* ActionTextBlock = Cast<UTextBlock>(GetWidgetFromName(Action));
		UTextBlock* RoleTextBlock = Cast<UTextBlock>(GetWidgetFromName(Role));
		UTextBlock* BetTextBlock = Cast<UTextBlock>(GetWidgetFromName(Bet));

		PlayerNames.Add(NameTextBlock);
		PlayerActions.Add(ActionTextBlock);
		PlayerRoles.Add(RoleTextBlock);
		PlayerBets.Add(BetTextBlock);
	}
}

void UTHWidget_InGame::RenderPlayerNames(TArray<FName> _PlayerNames)
{
	for (int32 i = 0; i < _PlayerNames.Num(); i++)
	{
		PlayerNames[i]->SetText(FText::FromName(_PlayerNames[i]));
	}
}

void UTHWidget_InGame::RenderPlayerBets(TArray<FName> _PlayerBets)
{
	for (int32 i = 0; i < _PlayerBets.Num(); i++)
	{
		if (_PlayerBets[i].IsEqual(TEXT("0")))
		{
			PlayerBets[i]->SetText(FText::FromName(TEXT("")));
		}
		else
		{
			PlayerBets[i]->SetText(FText::FromName(_PlayerBets[i]));
		}
	}
}

void UTHWidget_InGame::RenderPlayerActions(TArray<FName> _PlayerActions)
{
	for (int32 i = 0; i < _PlayerActions.Num(); i++)
	{
		PlayerActions[i]->SetText(FText::FromName(_PlayerActions[i]));
	}
}

void UTHWidget_InGame::RenderPlayerRoles(TArray<FName> _PlayerRoles)
{
	for (int32 i = 0; i < _PlayerRoles.Num(); i++)
	{
		if (_PlayerRoles[i].IsEqual(PokerRole::None))
		{
			PlayerRoles[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			PlayerRoles[i]->SetVisibility(ESlateVisibility::Visible);
		}

		PlayerRoles[i]->SetText(FText::FromName(_PlayerRoles[i]));
	}
}

void UTHWidget_InGame::RenderPlayerCards(TArray<UTexture2D*> _PlayerCards)
{
	for (int32 i = 0; i < _PlayerCards.Num(); i++)
	{
		PlayerCards[i]->SetBrushFromTexture(_PlayerCards[i], true);
	}
}

void UTHWidget_InGame::RenderCommunityCards(TArray<UTexture2D*> _CommunityCards)
{
	for (int32 i = 0; i < _CommunityCards.Num(); i++)
	{
		CommunityCards[i]->SetBrushFromTexture(_CommunityCards[i], true);
	}
}
