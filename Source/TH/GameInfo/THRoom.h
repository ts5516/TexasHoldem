// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/ActorChannel.h"
#include "THRoom.generated.h"

/**
* Lobby에 존재하는 Room에 대한 구조체
*/
USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	/* Room에 속해있는 PlayerController에 대한 배열*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	TArray<TObjectPtr<APlayerController>> Players;
	/* Room의 이름, 다른 방과 이름이 중복될 수 없다*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	FString Title = FString(TEXT("-"));
	/* 현재 Room에 속해 있는 인원 수*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	int32 NumOfPeople;*/
	/* Room에 들어가는 최대인원을 뜻하는 값으로, Room이 생성될 때 초기화되어야 한다.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	int32 Capacity = 8;

	void SetPlayers(const TArray<TObjectPtr<APlayerController>>& NewPlayers)
	{
		Players.Empty();

		for (auto& Player : NewPlayers)
		{
			Players.Add(Player);
		}
	}
};


/**
 * Lobby에 존재하는 Room Info Object
 */
UCLASS()
class TH_API ATHRoom : public AInfo
{
	GENERATED_BODY()

public:

	ATHRoom();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual bool IsSupportedForNetworking() const override;

private:

	UPROPERTY(Replicated)
	FRoomInfo RoomData;

public:

	const FRoomInfo& GetData() const { return RoomData; }
	void SetData(const FRoomInfo& NewRoomData);

	void AddPlayer(const TObjectPtr<APlayerController> NewPlayer) { RoomData.Players.Add(NewPlayer); }
	void RemovePlayer(const TObjectPtr<APlayerController> NewPlayer) { RoomData.Players.Remove(NewPlayer); }

};
