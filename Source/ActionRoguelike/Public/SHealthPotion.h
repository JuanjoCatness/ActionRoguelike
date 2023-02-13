// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUp.h"
#include "SHealthPotion.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickUp
{
	GENERATED_BODY()
	
public:
	ASHealthPotion();

protected:
	virtual void Interact(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly)
		float HealQuantity;

	UPROPERTY(EditDefaultsOnly)
		float CreditCost;
};
