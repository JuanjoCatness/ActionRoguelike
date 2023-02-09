// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include "SActionComp.h"

USActionEffect::USActionEffect(){
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* NewInstigator){
	Super::StartAction_Implementation(NewInstigator);

	if (Duration > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", NewInstigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f) {
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", NewInstigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* NewInstigator){
	//For not miss on the last tick
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) {
		ExecutePeriodEffect(NewInstigator);
	}

	Super::StopAction_Implementation(NewInstigator);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	USActionComp* Comp = GetOwningComponent();
	if (Comp) {
		Comp->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodEffect_Implementation(AActor* Insigator){

}
