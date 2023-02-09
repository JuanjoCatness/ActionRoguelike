// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTaskNode_Heal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeCompontent.h"

EBTNodeResult::Type USBTTaskNode_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (AIC) {
		USAttributeCompontent* AttComp = Cast<USAttributeCompontent>(AIC->GetPawn()->GetComponentByClass(USAttributeCompontent::StaticClass()));
		AttComp->ApplyHealthChange(nullptr, HealQuantity);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
