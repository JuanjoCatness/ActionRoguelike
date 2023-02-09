// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StartAction(AActor* NewInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		void StopAction(AActor* NewInstigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

	UWorld* GetWorld() const override;
};
