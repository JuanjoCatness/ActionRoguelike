// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeCompontent;
class UUSerWidget;
class USWorldyUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	USWorldyUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		FName BlackboardKey_HealthName;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName TimeToHitParam;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USAttributeCompontent* AttributeComp;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, USAttributeCompontent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateHealthBlackBoardKey();

	void SetTargetActor(AActor* Pawn);
};
