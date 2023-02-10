// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComp.h"
#include "Net/UnrealNetwork.h"

void USAction::StartAction_Implementation(AActor* NewInstigator){
	USActionComp* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = NewInstigator;
}

void USAction::StopAction_Implementation(AActor* NewInstigator){

	//ensureAlways(bIsRunning);

	USActionComp* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
}

USActionComp* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::OnRep_RepData(){
	if (RepData.bIsRunning) {
		StartAction(RepData.Instigator);
	}
	else {
		StopAction(RepData.Instigator);
	}
}

void USAction::Initialize(USActionComp* NewActionComp){
	ActionComp = NewActionComp;
}



bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* NewInstigator){
	if (IsRunning()) {
		return false;
	}

	USActionComp* Comp = GetOwningComponent();
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}
	return true;
}

UWorld* USAction::GetWorld() const
{
	//Changed compt to actor because unreal when replicates the action change the world because change from component to actor

	//Outer set via blueprint
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}

	return nullptr;
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Says to all clients to set the lidIsOpen
	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}