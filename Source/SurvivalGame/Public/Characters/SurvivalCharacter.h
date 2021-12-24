// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "SurvivalCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInventoryManager;
class UHotbarManager;
class USpringArmComponent;
class UInventoryHUDWidget;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(VisibleDefaultsOnly, Category = "Default|Mesh")
	USkeletalMeshComponent* Mesh_FirstPerson;

	UPROPERTY(VisibleDefaultsOnly, Category = "Default|Mesh")
	USkeletalMeshComponent* Mesh_ThirdPerson;

	UPROPERTY(VisibleDefaultsOnly, Category = "Default|Mesh")
	USpringArmComponent* DropPointSpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Default|Mesh")
	USceneComponent* DropPoint;

	UPROPERTY()
	FHitResult LastLookTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Mesh", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraAttachmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Camera", meta = (AllowPrivateAccess = "true"))//AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|UI", meta = (AllowPrivateAccess = "true"))
	UInventoryHUDWidget* InventoryHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|UI", meta = (AllowPrivateAccess = "true"))
	float DefaultMoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|UI", meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|UI", meta = (AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Control", meta = (AllowPrivateAccess = "true"))
	bool CanSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Control", meta = (AllowPrivateAccess = "true"))
	bool IsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Control", meta = (AllowPrivateAccess = "true"))
	bool IsSprintHeld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default|Control", meta = (AllowPrivateAccess = "true"))
	FVector2D MoveInput;


public:
	// Sets default values for this character's properties
	ASurvivalCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control")
	float UseDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Control")
	bool UseCrouchToggle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default|Inventory|UI")
	TSubclassOf<UInventoryHUDWidget> InventoryHUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Default|Control")
	bool GetCanSprint() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Control")
	void SetCanSprint(bool NewCanSprint);

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Inventory")
	UInventoryManager* InventoryManagerRef;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Default|Inventory")
	UHotbarManager* HotbarManagerRef;

	FInteractionInfo LastInteractionInfo;
	AActor* LastInteractionActorRef;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	USkeletalMeshComponent* GetMesh1P() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetInventoryHUDWidget(UInventoryHUDWidget* NewInventoryHUDWidget);

	UFUNCTION()
	void OnFire();
	
	UFUNCTION(BlueprintCallable, Category = Control)
	FHitResult GetLastLookTrace() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void OnOpenInventory();
	virtual void OnOpenInventory_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void OnUse();
	virtual void OnUse_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void OnStopUse();
	virtual void OnStopUse_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void StartCrouch();
	virtual void StartCrouch_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void StopCrouching();
	virtual void StopCrouching_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void StartSprint();
	virtual void StartSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void StopSprinting();
	virtual void StopSprinting_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void TrySprinting();
	virtual void TrySprinting_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void OnScrollUp();
	virtual void OnScrollUp_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Default|Control")
	void OnScrollDown();
	virtual void OnScrollDown_Implementation();

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void OnLinkedInventoryChanged();

	//void CameraYaw(float AxisValue);
	//void CameraPitch(float AxisValue);

};
