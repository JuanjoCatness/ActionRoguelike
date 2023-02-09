// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComp.h"

void USAction::StartAction_Implementation(AActor* NewInstigator){
	USActionComp* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* NewInstigator){

	ensureAlways(bIsRunning);

	USActionComp* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

USActionComp* USAction::GetOwningComponent() const
{
	return Cast<USActionComp>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
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
	//Outer set via blueprint
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp) {
		return Comp->GetWorld();
	}

	return nullptr;
}
