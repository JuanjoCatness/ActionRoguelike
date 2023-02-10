// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "SWorldyUserWidget.h"

void USInteractionComponent::PrimaryInteract() {
	ServerInteract(FocusedActor); //Server PCB

}

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceDistance = 1000.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}


void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus){

	if (InFocus == nullptr) {
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);


}

void USInteractionComponent::FindBestInteractable(){
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector Start;

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FocusedActor = nullptr;

	for (FHitResult Hit : Hits) {
		if (AActor* HitActor = Hit.GetActor()) {
			if (HitActor->Implements<USGameplayInterface>()) {
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor) {
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass)) {
			DefaultWidgetInstance = CreateWidget<USWorldyUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport()) {
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else {
		if (DefaultWidgetInstance) {
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());

	//Limits who execute this code, in this case only the client
	if (MyPawn->IsLocallyControlled()) {
		FindBestInteractable();
	}
}

