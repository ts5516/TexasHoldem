// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TH/GameState/THGameState_InGame.h"
#include "THWidget_InGame.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class TH_API UTHWidget_InGame : public UUserWidget
{
	GENERATED_BODY()

public:

	UTHWidget_InGame(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	TArray<UTextBlock*> PlayerNames;

	UPROPERTY()
	TArray<UTextBlock*> PlayerBets;

	UPROPERTY()
	TArray<UTextBlock*> PlayerActions;

	UPROPERTY()
	TArray<UTextBlock*> PlayerRoles;

	UPROPERTY()
	TArray<UImage*> PlayerCards;

	UPROPERTY()
	TArray<UImage*> CommunityCards;

public:

	UFUNCTION(BlueprintCallable)
	void RenderPlayerNames(TArray<FName> _PlayerNames);

	UFUNCTION(BlueprintCallable)
	void RenderPlayerBets(TArray<FName> _PlayerBets);

	UFUNCTION(BlueprintCallable)
	void RenderPlayerActions(TArray<FName> _PlayerActions);

	UFUNCTION(BlueprintCallable)
	void RenderPlayerRoles(TArray<FName> _PlayerRoles);

	UFUNCTION(BlueprintCallable)
	void RenderPlayerCards(TArray<UTexture2D*> _PlayerCards);

	UFUNCTION(BlueprintCallable)
	void RenderCommunityCards(TArray<UTexture2D*> _CommunityCards);
public:

	ATHGameState_InGame* GetGameState() const
	{
		return  GetWorld() ? Cast<ATHGameState_InGame>(GetWorld()->GetGameState()) : nullptr;
	}
};
