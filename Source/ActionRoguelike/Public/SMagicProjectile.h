// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBaseClass.h"
#include "GameplayTagContainer.h"
#include "SMagicProjectile.generated.h"

class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectileBaseClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TSubclassOf<USActionEffect> BurningActionClass;

	UFUNCTION()
		void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
