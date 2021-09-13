// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/StatusInterface.h"
#include "Characters/StatusTypes.h"
#include "BasicDoor.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	IsClosed	UMETA(DisplayName = "Closed"),
	IsOpen		UMETA(DisplayName = "Open"),
	IsClosing	UMETA(DisplayName = "Opening"),
	IsOpening	UMETA(DisplayName = "Closing"),
};

UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class SURVIVALGAME_API ABasicDoor : public AActor, public IInteractionInterface, public IStatusInterface
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door", meta = (AllowPrivateAccess = "true"))
	EDoorState CurrentDoorState = EDoorState::IsClosed;
	
public:
	// Sets default values for this actor's properties
	ABasicDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door|Status", meta = (BlueprintProtected))
	EHealthState StatusHealthState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door|Status", meta = (BlueprintProtected))
	float StatusHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door|Status", meta = (BlueprintProtected))
	float StatusHealth_MAX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door", meta = (BlueprintProtected))
	bool OpensLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door", meta = (BlueprintProtected))
	FTransform DoorOpenTransform;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	EDoorState GetDoorState() const;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void SetDoorState(const EDoorState NewCurrentDoorState);

	bool CanInteract_Implementation(const FInteractionInfo Info) const override;
	void BeginInteraction_Implementation(const FInteractionInfo Info) override;

	bool CanModifyStatus_Implementation(const EStatusType StatusType) const override;
	float GetStatus_Implementation(const EStatusType StatusType) const override;
	void ModifyStatus_Implementation(const EStatusType StatusType, const float Amount) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnOpen();
	void OnOpen_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnClose();
	void OnClose_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnBreakDoor();
	void OnBreakDoor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnRepairDoor();
	void OnRepairDoor_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnStartPeek();
	void OnStartPeek_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void OnEndPeek();
	void OnEndPeek_Implementation();
};
