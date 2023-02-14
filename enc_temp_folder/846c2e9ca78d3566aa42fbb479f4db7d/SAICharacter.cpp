// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeCompontent.h"
#include "BrainComponent.h"
#include "SWorldyUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComp.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComp = CreateDefaultSubobject <USAttributeCompontent>(TEXT("AttributeComponent"));
	ActionComp = CreateDefaultSubobject <USActionComp>(TEXT("ActionComp"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	BlackboardKey_HealthName = "Health";
	TimeToHitParam = "TimeToHit";
}

void ASAICharacter::PostInitializeComponents(){
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChange.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeCompontent* OwningComp, float NewHealth, float Delta){
	if (Delta < 0.0f) {

		if (InstigatorActor != this) {
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr) {
			ActiveHealthBar = CreateWidget<USWorldyUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar) {
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}



		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParam, GetWorld()->TimeSeconds);

		//Died
		if (NewHealth <= 0.0f) {
			//stop bt
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC) {
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//set lifespan
			SetLifeSpan(10.0f);

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
		}

		UpdateHealthBlackBoardKey();
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn){
	SetTargetActor(Pawn);
}

void ASAICharacter::UpdateHealthBlackBoardKey(){
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC) {
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		if (BBComp) {
			BBComp->SetValueAsFloat(BlackboardKey_HealthName, AttributeComp->GetHealth());
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* Pawn){
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && Pawn) {
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}


// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBlackBoardKey();
}
