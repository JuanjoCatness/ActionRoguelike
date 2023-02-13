// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/Datatable.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSavesGame;
class UDataTable;
class USMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase {
	GENERATED_BODY()

public:

	FMonsterInfoRow() {
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FPrimaryAssetId MonsterId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float KillReward;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	FString SlotName;
	UPROPERTY()
		USSavesGame* CurrentSaveGame;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SpawnTimerInterval;

	UPROPERTY(EditAnywhere, Category = "AI")
		UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditAnywhere, Category = "AI")
		UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<AActor> MinionClass;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnedLocation);

	UFUNCTION()
		void RespawnPlayerElapsed(AController* Controller);

public:
	ASGameModeBase();
	virtual void StartPlay() override;

	UFUNCTION(Exec)
		void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void WriteSaveGame();

	void LoadSaveGame();
};
