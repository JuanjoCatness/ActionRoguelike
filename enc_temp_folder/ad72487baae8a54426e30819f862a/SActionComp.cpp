// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComp.h"
#include "SAction.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


// Sets default values for this component's properties
USActionComp::USActionComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void USActionComp::ServerStopAction_Implementation(AActor* Instigator, FName ActionName){
	StopActionByName(Instigator, ActionName);
}

void USActionComp::ServerStartAction_Implementation(AActor* Instigator, FName ActionName){
	StartActionByName(Instigator, ActionName);
}


void USActionComp::BeginPlay()
{
	Super::BeginPlay();
	
	//Server only
	if (GetOwner()->HasAuthority()) {
		for (TSubclassOf<USAction> ActionClass : DefaultsActions) {
			AddAction(GetOwner(), ActionClass);
		}
	}
}


bool USActionComp::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{

	//Check if its something its wrote/change if that the case says "hey replicate this too"
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	//Iterates over all actions to say to unreal that has a list to replicate
	for (USAction* Action : Actions) {
		if (Action) {
			//Check if any of the variables has changes
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USActionComp::AddAction(AActor* NewInstigator, TSubclassOf<USAction> ActionClass){
	if (!ensure(ActionClass)) {
		return;
	}

	//Skip for clients
	if (!GetOwner()->HasAuthority()) {
		return;
	}

	//this its the owner
	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction)) {
		NewAction->Initialize(this);

		Actions.Add(NewAction);
		
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(NewInstigator))) {
			NewAction->StartAction(NewInstigator);
		}
	}
}

void USActionComp::RemoveAction(USAction* ActionToRemove){
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) {
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool USActionComp::StartActionByName_Implementation(AActor* NewInstigator, FName ActionName){
	for (USAction* Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (!Action->CanStart(NewInstigator)) {
				continue;
			}

			//Is client?
			if (!GetOwner()->HasAuthority()) {
				ServerStartAction(NewInstigator, ActionName);
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
				//Is client?
				if (!GetOwner()->HasAuthority()) {
					ServerStopAction(NewInstigator, ActionName);
				}

				Action->StopAction(NewInstigator);
				return true;
			}
		}
	}

	return false;
}

void USActionComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Says to all clients to set the lidIsOpen
	DOREPLIFETIME(USActionComp, Actions);
}