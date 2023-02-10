#include "SSaveGame.h"

// Establece valores predeterminados
ASSaveGame::ASSaveGame()
{
	// Establezca esta actor para llamar. Marca() cada fotograma. Puede desactivar esta opción para mejorar el rendimiento si no lo necesita.
	PrimaryActorTick.bCanEverTick = true;
}

// Se llama cuando se inicia el juego o cuando se genera
void ASSaveGame::BeginPlay()
{
	Super::BeginPlay();
}

// Llamada a cada fotograma
void ASSaveGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
