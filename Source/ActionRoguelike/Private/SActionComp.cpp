// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComp.h"
#include "SAction.h"


// Sets default values for this component's properties
USActionComp::USActionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USActionComp::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultsActions) {
		AddAction(ActionClass);
	}
	
}


void USActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USActionComp::AddAction(TSubclassOf<USAction> ActionClass){
	if (!ensure(ActionClass)) {
		return;
	}
	//this its the owner
	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (ensure(NewAction)) {
		Actions.Add(NewAction);
	}
}

bool USActionComp::StartActionByName_Implementation(AActor* NewInstigator, FName ActionName){
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (!Action->CanStart(NewInstigator)) {
				continue;
			}

			Action->StartAction(NewInstigator);
			return true;
		}
	}

	return false;
}

bool USActionComp::StopActionByName_Implementation(AActor* NewInstigator, FName ActionName)
{
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->IsRunning()) {
				Action->StopAction(NewInstigator);
				return true;
			}
		}
	}

	return false;
}