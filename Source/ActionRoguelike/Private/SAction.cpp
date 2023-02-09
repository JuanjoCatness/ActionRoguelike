// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* NewInstigator){

}

void USAction::StopAction_Implementation(AActor* NewInstigator){

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
