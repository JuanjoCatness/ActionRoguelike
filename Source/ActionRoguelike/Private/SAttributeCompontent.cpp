// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeCompontent.h"
#include "Math/UnrealMathUtility.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<bool> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier."), ECVF_Cheat);

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

	SetIsReplicatedByDefault(true);
}

USAttributeCompontent* USAttributeCompontent::GetAttribute(AActor* Actor)
{
	if (Actor) {
		return Cast<USAttributeCompontent>(Actor->GetComponentByClass(USAttributeCompontent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeCompontent::IsActorAlive(AActor* Actor)
{
	USAttributeCompontent* AttComp = GetAttribute(Actor);
	if (AttComp) {
		return AttComp->IsAlive();
	}
	return false;
}

bool USAttributeCompontent::Kill(AActor* InsitigatorActor){
	return ApplyHealthChange(InsitigatorActor, -GetMaxHealth());
}

bool USAttributeCompontent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f) {
		return false;
	}

	if (Delta < 0.0f) {
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	Health += Delta;
	Health = FMath::Clamp(Health, 0, HealthMax);

	float ActualDelta = Health - OldHealth;

	//OnHealthChange.Broadcast(InstigatorActor, this, Health, ActualDelta);
	if (Delta != 0.0f) {
		MulticastHealthChange(InstigatorActor, Health, ActualDelta);
	}

	if (ActualDelta < 0.0f && Health == 0.0f) {
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

float USAttributeCompontent::GetHealth()
{
	return Health;
}

float USAttributeCompontent::GetMaxHealth()
{
	return HealthMax;
}

void USAttributeCompontent::MulticastHealthChange_Implementation(AActor* InstigatorActor, float NewHealth, float Delta){
	OnHealthChange.Broadcast(InstigatorActor,this, NewHealth, Delta);
}

void USAttributeCompontent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Says to all clients to set the lidIsOpen
	DOREPLIFETIME(USAttributeCompontent, Health);
	DOREPLIFETIME(USAttributeCompontent, HealthMax);
	
	//This is for a optimization
	//COND_OwnerOnly -> Only the owner can see the change
	//COND_InitialOnly -> Only when spawns
	//DOREPLIFETIME_CONDITION(USAttributeCompontent, HealthMax, COND_OwnerOnly);
}
