// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeCompontent.h"

ASHealthPotion::ASHealthPotion()
{
	HealQuantity = 20.0f;
}

void ASHealthPotion::Interact(APawn* InstigatorPawn){
	USAttributeCompontent* AttributeComponent = Cast<USAttributeCompontent>(InstigatorPawn->GetComponentByClass(USAttributeCompontent::StaticClass()));

	if (AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth()) {
		Super::Interact(InstigatorPawn);
		AttributeComponent->ApplyHealthChange(this, HealQuantity);
	}
}

