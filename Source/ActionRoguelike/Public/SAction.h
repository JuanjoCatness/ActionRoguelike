// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComp;

USTRUCT()
struct FActionWrapData {
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;

};

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
		USActionComp* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComp* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
		FActionWrapData RepData;
	
	UFUNCTION()
	void OnRep_RepData();

public:
	void Initialize(USActionComp* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool CanStart(AActor* NewInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* NewInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* NewInstigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

	UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override;
};
