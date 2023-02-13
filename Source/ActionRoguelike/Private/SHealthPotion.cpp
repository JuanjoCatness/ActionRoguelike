// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeCompontent.h"


#define LOCTEXT_NAMESPACE "IteractableActors"

ASHealthPotion::ASHealthPotion()
{
	HealQuantity = 20.0f;
	CreditCost = 10.0f;
}

void ASHealthPotion::Interact(APawn* InstigatorPawn){
	USAttributeCompontent* AttributeComponent = Cast<USAttributeCompontent>(InstigatorPawn->GetComponentByClass(USAttributeCompontent::StaticClass()));

	if (AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth()) {
		Super::Interact(InstigatorPawn);
		AttributeComponent->ApplyHealthChange(this, HealQuantity);
	}
}

FText ASHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeCompontent* AttComp = USAttributeCompontent::GetAttribute(InstigatorPawn);
	if (AttComp && AttComp->IsFullHealth()) {
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_FullHealthWarning", "Cost {0} Credits. Restores health"), CreditCost);
}

#undef LOCTEXT_NAMESPACE

