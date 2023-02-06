// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBaseClass.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
ASProjectileBaseClass::ASProjectileBaseClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ShpereComp = CreateDefaultSubobject<USphereComponent>("ShereComp");
	ShpereComp->SetCollisionProfileName("Projectile");
	ShpereComp->IgnoreActorWhenMoving(GetInstigator(),true);
	RootComponent = ShpereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(ShpereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(ShpereComp);
	AudioComp->Activate();

	ImpactShakeInnerRadius = 3000.0f;
	ImpactShakeOuterRadius = 8000.0f;

}

// Called when the game starts or when spawned
void ASProjectileBaseClass::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASProjectileBaseClass::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OhterActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	Explode();
}

void ASProjectileBaseClass::Explode_Implementation(){
	if (ensure(IsValid(this))) {
		AudioComp->Stop();
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		Destroy();
	}
}

void ASProjectileBaseClass::PostInitializeComponents() {
	Super::PostInitializeComponents();
	ShpereComp->OnComponentHit.AddDynamic(this, &ASProjectileBaseClass::OnActorHit);
}

// Called every frame
void ASProjectileBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

