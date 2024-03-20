#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TH/GameState/THGameState_Lobby.h"
#include "Components/TextBlock.h"
#include "THWidget_Room.generated.h"

class UTextBlock;
class UTileView;

UCLASS()
class TH_API UTHWidget_Room : public UUserWidget
{
	GENERATED_BODY()

public:

	UTHWidget_Room(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NumOfPeople;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Capacity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTileView> PlayerTileView;

public:

	UFUNCTION()
	void OnUpdatedRoom();

	UFUNCTION(BlueprintCallable)
	void RenderRoom(ATHRoom* Room);

	void InitRoom();

public:

	ATHGameState_Lobby* GetGameState() { return GetWorld()->GetGameState<ATHGameState_Lobby>(); }
};
