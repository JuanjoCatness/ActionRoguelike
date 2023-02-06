// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBaseClass.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticlesSystemComponent;
class UAudioComponent;
class UCameraShakeBase;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASProjectileBaseClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBaseClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OhterActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		float ImpactShakeOuterRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* ShpereComp;

	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere)
		USoundBase* ImpactSound;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Explode();

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
