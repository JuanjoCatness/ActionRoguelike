// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeCompontent.h"
#include "Math/UnrealMathUtility.h"

bool USAttributeCompontent::IsAlive() const{
	return Health > 0.0f;
}

// Sets default values for this component's properties
USAttributeCompontent::USAttributeCompontent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	HealthMax = 100.0f;
	Health = HealthMax;
}

bool USAttributeCompontent::ApplyHealthChange(float Delta)
{

	Health += Delta;
	Health = FMath::Clamp(Health, 0, HealthMax);
	OnHealthChange.Broadcast(nullptr, this, Health, Delta);

	return true;
}

float USAttributeCompontent::GetHealth()
{
	return Health;
}

float USAttributeCompontent::GetMaxHealth()
{
	return HealthMax;
}
