// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComp.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool StartActionByName(AActor* NewInstigator, FName ActionName);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool StopActionByName(AActor* NewInstigator, FName ActionName);

	// Sets default values for this component's properties
	USActionComp();

protected:

	UPROPERTY(EditAnywhere, Category="Actions")
		TArray<TSubclassOf<USAction>> DefaultsActions;

	UPROPERTY()
		TArray<USAction*> Actions;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
