// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPickUp.generated.h"

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASPickUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUp();

	void Interact_Implementation(APawn* InstigatorPawn);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
		float TimeToReactivate;

	virtual void Interact(APawn* InstigatorPawn);
	void Enable();
	void Disable();

	bool bCanInteract;

	FTimerHandle TimerHandle_PrimaryAttack;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
