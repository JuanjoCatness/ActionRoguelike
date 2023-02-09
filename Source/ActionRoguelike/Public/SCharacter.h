// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeCompontent;
class UAnimMontage;
class UParticleSystem;
class USActionComp;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	UFUNCTION(Exec)
		void HealSelf(float Quantity = 100.0f);

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
		USAttributeCompontent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USActionComp* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	void PrimaryAttack();

	void BlackHoleAttack();


	void Dash();


	void PrimaryInteract();


	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeCompontent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
