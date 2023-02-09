// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldyUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldyUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor)) {
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("No more valid attached actor"));
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffeset, ScreenPosition)) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (ParentSizeBox) {
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
