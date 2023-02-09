// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldyUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<USWorldyUserWidget> DefaultWidgetClass;

	UPROPERTY()
		USWorldyUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TEnumAsByte<ECollisionChannel> CollisionChannel;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
