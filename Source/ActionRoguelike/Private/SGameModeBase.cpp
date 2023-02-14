// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/SAICharacter.h"
#include "SAttributeCompontent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SSavesGame.h"
#include "Kismet/GameplayStatics.h"
#include "SGameplayInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SMonsterData.h"
#include "SActionComp.h"
#include "Engine/AssetManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.Spawnbots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase(){
	SpawnTimerInterval = 2.0f;

	SlotName = "Slot_01";
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

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage){
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");

	if (SelectedSaveSlot.Len() > 0) {
		SlotName = SelectedSaveSlot;
	}
	else {
		SlotName = "DefaultSlotSaveGame";
	}

	LoadSaveGame();
}

void ASGameModeBase::WriteSaveGame(){

	CurrentSaveGame->SavedActor.Empty();

	for (FActorIterator It(GetWorld()); It; ++It) {
		AActor* Actor = *It;

		if (!Actor->Implements<USGameplayInterface>()) {
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		//Serialize the object and saves it
			FMemoryWriter MemWriter(ActorData.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			//Find only variables with UPROPERTY(SaveGame)
			Ar.ArIsSaveGame = true;
			Actor->Serialize(Ar);
		//---

		CurrentSaveGame->SavedActor.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame() {
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0)) {
		CurrentSaveGame = Cast<USSavesGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr) {
			return;
		}

		for (FActorIterator It(GetWorld()); It; ++It) {
			AActor* Actor = *It;

			if (!Actor->Implements<USGameplayInterface>()) {
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActor) {
				if (ActorData.ActorName == Actor->GetName()) {
					Actor->SetActorTransform(ActorData.Transform);

					//Serialize the object and loads it
					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					//Convert binary array back into actors variables
					Actor->Serialize(Ar);
					//---

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
				}
			}
		}

	}
	else {
		CurrentSaveGame = Cast<USSavesGame>(UGameplayStatics::CreateSaveGameObject(USSavesGame::StaticClass()));
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

		if (MonsterTable) {
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			//Get random row
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];


			//Carga el asset de forma asincrona
			//Get asset manager
			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager) {
				TArray<FName>Bundles;
				//Create delegate which will trigger the OnMonsterLoaded once the asset its loaded
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);
				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}

		
	}
}

/*This function its what actually spawns*/
void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnedLocation){

	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager) {
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));

		if (MonsterData) {
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnedLocation, FRotator::ZeroRotator);

			if (NewBot) {
				USActionComp* ActionComp = Cast<USActionComp>(NewBot->GetComponentByClass(USActionComp::StaticClass()));

				for (TSubclassOf<USAction> ActionClass : MonsterData->Actions) {
					ActionComp->AddAction(NewBot, ActionClass);
				}
			}
		}
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
