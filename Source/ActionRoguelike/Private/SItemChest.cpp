// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn){
	
	bLidIsOpen = !bLidIsOpen;
	OnRep_LidOpened();

}

void ASItemChest::OnActorLoaded_Implementation(){
	OnRep_LidOpened();
}

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110.0;

	SetReplicates(true);
}

void ASItemChest::OnRep_LidOpened(){
	float CurrPitch = bLidIsOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Says to all clients to set the lidIsOpen
	DOREPLIFETIME(ASItemChest, bLidIsOpen);
}
