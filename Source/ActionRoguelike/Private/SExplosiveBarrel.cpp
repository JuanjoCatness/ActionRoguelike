// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "SAttributeCompontent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	//RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	RadialForceComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents(){
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnCompHit);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
	if (OtherActor) {
		USAttributeCompontent* AttributeComp = Cast<USAttributeCompontent>(OtherActor->GetComponentByClass(USAttributeCompontent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(this, -50);
		}
	}
}

