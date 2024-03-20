// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/ActorChannel.h"
#include "THRoom.generated.h"

/**
* Lobby�� �����ϴ� Room�� ���� ����ü
*/
USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	/* Room�� �����ִ� PlayerController�� ���� �迭*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	TArray<TObjectPtr<APlayerController>> Players;
	/* Room�� �̸�, �ٸ� ��� �̸��� �ߺ��� �� ����*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	FString Title = FString(TEXT("-"));
	/* ���� Room�� ���� �ִ� �ο� ��*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	int32 NumOfPeople;*/
	/* Room�� ���� �ִ��ο��� ���ϴ� ������, Room�� ������ �� �ʱ�ȭ�Ǿ�� �Ѵ�.*/
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
 * Lobby�� �����ϴ� Room Info Object
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
