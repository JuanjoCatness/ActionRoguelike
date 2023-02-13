// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUp.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPickUp::ASPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	//bCanInteract = true;
	TimeToReactivate = 10.0f;

	SetReplicates(true);
}

void ASPickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	Interact(InstigatorPawn);
}


void ASPickUp::Interact(APawn* InstigatorPawn){
	//if (bCanInteract) {
		Disable();
		//Other logic
		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASPickUp::Enable, TimeToReactivate, false);
	//}
}

FText ASPickUp::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

// Called when the game starts or when spawned
void ASPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickUp::Enable(){
	//bCanInteract = true;
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BaseMesh->SetVisibility(true);
}

void ASPickUp::Disable(){
	//bCanInteract = false;
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetVisibility(false);
}

