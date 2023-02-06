// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeCompontent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InsigatorActor, USAttributeCompontent*,OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeCompontent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
	
	// Sets default values for this component's properties
	USAttributeCompontent();


public:	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChange;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

	float GetHealth();
	float GetMaxHealth();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float HealthMax;
};
