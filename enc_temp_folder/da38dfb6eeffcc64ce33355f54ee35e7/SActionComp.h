// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComp.generated.h"

class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, USActionComp*, OwningComp, USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(AActor* NewInstigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		bool StartActionByName(AActor* NewInstigator, FName ActionName);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
		bool StopActionByName(AActor* NewInstigator, FName ActionName);

	// Sets default values for this component's properties
	USActionComp();

protected:
	UFUNCTION(Server, Reliable)
		void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
		void ServerStopAction(AActor* Instigator, FName ActionName);


	UPROPERTY(EditAnywhere, Category="Actions")
		TArray<TSubclassOf<USAction>> DefaultsActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<USAction*> Actions;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable)
		FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
		FOnActionStateChanged OnActionStopped;


	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
