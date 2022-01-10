// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/BasicDoor.h"
#include "DamageType_HealMechanical.h"

// Sets default values
ABasicDoor::ABasicDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorOpenTransform = FTransform();
	StatusHealth = 100.f;
	StatusHealth_MAX = 100.f;
}

// Called when the game starts or when spawned
void ABasicDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

inline EDoorState ABasicDoor::GetDoorState() const { return CurrentDoorState; }

inline void ABasicDoor::SetDoorState(const EDoorState NewCurrentDoorState) { CurrentDoorState = NewCurrentDoorState; }

bool ABasicDoor::CanInteract_Implementation(const FInteractionInfo Info) const
{
	return GetDoorState() == EDoorState::IsOpen || GetDoorState() == EDoorState::IsClosed;
}

void ABasicDoor::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	if (GetDoorState() == EDoorState::IsOpen) {
		OnClose();
	}
	else {
		OnOpen();
	}
}

bool ABasicDoor::CanModifyStatus_Implementation(const EStatusType StatusType) const
{
	return false;
}

float ABasicDoor::GetStatus_Implementation(const EStatusType StatusType) const
{
	return 0.0f;
}

void ABasicDoor::ModifyStatus_Implementation(const EStatusType StatusType, const float Amount)
{
}

float ABasicDoor::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// Inverts damage if DamageType is mechanical
	if (DamageEvent.DamageTypeClass == UDamageType_HealMechanical::StaticClass()) {
		DamageAmount *= -1;
	}
	StatusHealth = FMath::Clamp(StatusHealth - DamageAmount, 0.f, StatusHealth_MAX);
	if (StatusHealthState == EHealthState::IsAlive) {
		if (StatusHealth == 0.f) {
			OnBreakDoor();
		}
	}
	else if(StatusHealthState == EHealthState::IsDead) {
		if (StatusHealth >= StatusHealth_MAX) {
			OnRepairDoor();
		}
	}
	return DamageAmount;
}

void ABasicDoor::OnOpen_Implementation()
{
	SetDoorState(EDoorState::IsOpening);
}

void ABasicDoor::OnClose_Implementation()
{
	SetDoorState(EDoorState::IsClosing);
}

void ABasicDoor::OnBreakDoor_Implementation()
{
	StatusHealthState = EHealthState::IsDying;
}

void ABasicDoor::OnRepairDoor_Implementation()
{
	StatusHealthState = EHealthState::IsRessing;
}

void ABasicDoor::OnStartPeek_Implementation()
{
}

void ABasicDoor::OnEndPeek_Implementation()
{
}

