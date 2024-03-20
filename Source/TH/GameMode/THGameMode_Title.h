// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "THGameMode.h"
#include "THGameMode_Title.generated.h"

/**
 * 
 */
UCLASS()
class TH_API ATHGameMode_Title : public ATHGameMode
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void TravelToLobby(int32 ControllerId);

};
