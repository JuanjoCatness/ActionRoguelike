// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmCOmponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "SAttributeCompontent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAction.h"
#include "SActionComp.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject <USAttributeCompontent>(TEXT("AttributeComponent"));

	ActionComp = CreateDefaultSubobject <USActionComp>(TEXT("ActionComponent"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

void ASCharacter::HealSelf(float Quantity){
	AttributeComp->ApplyHealthChange(this, Quantity);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASCharacter::MoveForward(float Value) {
	//Moves the character at the direction of the camera (more specifically the controller)
	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value){
	//Obtains the roation of the camera for right vector

	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.0f;
	ControlRot.Pitch = 0.0f;
	// X = forward (red)
	// Y = right (green)
	// Z = up (blue)
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::SprintStart(){
	ActionComp->StartActionByName(this, "sprint");
}

void ASCharacter::SprintStop(){
	ActionComp->StopActionByName(this, "sprint");
}

void ASCharacter::PrimaryAttack(){
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ASCharacter::BlackHoleAttack(){
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}


void ASCharacter::Dash(){
	ActionComp->StartActionByName(this, "Dash");
}


void ASCharacter::PrimaryInteract(){
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}


void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeCompontent* OwningComp, float NewHealth, float Delta){
	if (Delta < 0.0f) {
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::PostInitializeComponents(){
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChange.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);


	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

