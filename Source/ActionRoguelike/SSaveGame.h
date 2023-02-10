#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSaveGame.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASSaveGame : public AActor
{
	GENERATED_BODY()
	
public:
	// Establece los valores predeterminados para las propiedades de este actor
	ASSaveGame();

protected:
	// Se llama cuando se inicia el juego o cuando se genera
	virtual void BeginPlay() override;

public:
	// Llamada a cada fotograma
	virtual void Tick(float DeltaTime) override;
};
