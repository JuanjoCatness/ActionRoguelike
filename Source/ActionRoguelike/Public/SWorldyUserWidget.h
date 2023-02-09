// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldyUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere, Category = "UI")
		FVector WorldOffeset;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta=(ExposeOnSpawn = true))
		AActor* AttachedActor;
};
