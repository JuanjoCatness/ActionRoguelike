// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void USAction_ProjectileAttack::AttackDelay_TimeElapsed(ACharacter* InstigatorCharacter){
	if (ensure(ProjectileClass)) {
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocket);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//Set the owner of the projectile as this character
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParam;
		ObjParam.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParam.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParam.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 50000);

		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParam, Shape, Params)) {
			TraceEnd = Hit.ImpactPoint;
		}

		FTransform SpawnTM = FTransform(FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator(), HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator){
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character) {
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastParticles, Character->GetMesh(), HandSocket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
		if (Character->HasAuthority()) {
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;

			Delegate.BindUFunction(this, "AttackDelay_TimeElapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
		}
	}
}

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocket = "Muzzle_01";
	AttackDelay = 0.2f;
}
