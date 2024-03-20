// Fill out your copyright notice in the Description page of Project Settings.


#include "THCard.h"
#include "Net/UnrealNetwork.h"

UTHCard::UTHCard()
{
}

void UTHCard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTHCard, bShowCardFront);
	DOREPLIFETIME(UTHCard, CardName);
}

FString UTHCard::GetCardValue()
{
	FString SCardName = CardName.ToString();
	FString CardValue;
	CardValue.AppendChar(SCardName[SCardName.Len() - 1]);

	return CardValue;
}

FString UTHCard::GetCardMark()
{
	FString CardValue;
	CardValue.AppendChar(CardName.ToString()[0]);

	return CardValue;
}

bool UTHCard::Comparator(const UTHCard& Card1, const UTHCard& Card2)
{
	FString PokerNumbers = "234567890JQKA";

	FString SCard1 = Card1.GetCardName().ToString();
	FString SCard2 = Card2.GetCardName().ToString();

	FString CardNum1, CardNum2;
	CardNum1.AppendChar(SCard1[SCard1.Len() - 1]);
	CardNum2.AppendChar(SCard2[SCard2.Len() - 1]);

	int32 Card1Index = PokerNumbers.Find(CardNum1);
	int32 Card2Index = PokerNumbers.Find(CardNum2);

	return Card1Index > Card2Index;
}
