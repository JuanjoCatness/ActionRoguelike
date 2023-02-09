// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeCompontent.h"
#include "EngineUtils.h"
#include "SCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.Spawnbots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase(){
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay(){
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll(){
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;
		USAttributeCompontent* AttComp = USAttributeCompontent::GetAttribute(Bot);
		if (AttComp && AttComp->IsAlive()) {
			AttComp->Kill(this);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer){
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player) {
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, 2.0f, false);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed() {

	if (!CVarSpawnBots.GetValueOnGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("Diabled"));
		return;
	}

	//Checks if can spawn, if not avoids the expensive query
	int32 NroOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;
		USAttributeCompontent* AttComp = USAttributeCompontent::GetAttribute(Bot);
		if (AttComp && AttComp->IsAlive()) {
			NroOfAliveBots += 1;
		}
	}

	float MaxCountCount = 10.0f;

	if (ensureMsgf(DifficultyCurve, TEXT("Difficulty curve not assigned"))) {
		MaxCountCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NroOfAliveBots >= MaxCountCount) {
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensureMsgf(QueryInstance, TEXT("Check if the SpawnBotQuery it's assigned"))) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus){
	//Few frames after the query its runned
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0)) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller){
	if (ensure(Controller)) {
		//Desposee el character
		Controller->UnPossess();
		//Crea una copia del personaje
		RestartPlayer(Controller);
	}
}
