// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckLowHealth.h"
#include "AIController.h"
#include "SAttributeCompontent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds){
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController)) {
		USAttributeCompontent* AttComp = Cast<USAttributeCompontent>(MyController->GetComponentByClass(USAttributeCompontent::StaticClass()));
		UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
		if (BlackBoardComp && AttComp) {
			BlackBoardComp->SetValueAsFloat(Health.SelectedKeyName, AttComp->GetHealth());
		}
	}
}
