// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "THCard.generated.h"

/**
 * 
 */
UCLASS()
class TH_API UTHCard : public UObject
{
	GENERATED_BODY()

public:

	UTHCard();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable)
	bool IsCardFront() const { return bShowCardFront; }
	void PutOnTheFront() { bShowCardFront = true; }
	void PutOnTheBack() { bShowCardFront = false; }

	UFUNCTION(BlueprintCallable)
	FName GetCardName() const { return CardName; }
	void SetCardName(FName& _CardName) { CardName = _CardName; }

	FString GetCardValue();
	FString GetCardMark();

	static bool Comparator(const UTHCard& Card1, const UTHCard& Card2);
	

private:

	UPROPERTY(Replicated)
	bool bShowCardFront = true;

	UPROPERTY(Replicated)
	FName CardName;

};
