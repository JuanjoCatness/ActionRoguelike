// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "SAttributeCompontent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeCompontent>("AttributeComp");
	AttributeComp->OnHealthChange.AddDynamic(this, &ASTargetDummy::OnHealthChanged);

}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeCompontent* OwningComp, float NewHealth, float Delta) {
	if (Delta < 0.0f){
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
