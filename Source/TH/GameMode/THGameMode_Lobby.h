// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THGameMode.h"
#include "THGameMode_Lobby.generated.h"

UCLASS()
class TH_API ATHGameMode_Lobby : public ATHGameMode
{
	GENERATED_BODY()

public:
	ATHGameMode_Lobby();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};