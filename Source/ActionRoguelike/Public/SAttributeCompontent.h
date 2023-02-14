// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeCompontent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InsigatorActor, USAttributeCompontent*,OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeCompontent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeCompontent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
	
	// Sets default values for this component's properties
	USAttributeCompontent();

	UFUNCTION(BlueprintCallable, Category ="Attributes")
	static USAttributeCompontent* GetAttribute(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta=(DisplayName = "IsAlive"))
		static bool IsActorAlive(AActor* Actor);

public:	
	UFUNCTION()
		bool Kill(AActor* InsitigatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
		FOnAttributeChanged OnRageChanged;


	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
		float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool ApplyRage(AActor* InstigatorActor, float Delta);


	bool IsFullHealth();

	float GetHealth();
	float GetMaxHealth();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attributes")
		float HealthMax;

	/* Resource used to power certain Actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
		float RageMax;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChange(AActor* InstigatorActor, float NewHealth, float Delta);
};
