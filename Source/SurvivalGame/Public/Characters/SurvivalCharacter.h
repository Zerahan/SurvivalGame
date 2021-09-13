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
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh_FirstPerson;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh_ThirdPerson;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USpringArmComponent* DropPointSpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* DropPoint;

	UPROPERTY()
	FHitResult LastLookTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraAttachmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))//AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	UInventoryHUDWidget* InventoryHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	float DefaultMoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	float SprintSpeedMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	float WalkSpeedMultiplier;


public:
	// Sets default values for this character's properties
	ASurvivalCharacter();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Control")
	float UseDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Control")
	bool UseCrouchToggle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory|UI")
	TSubclassOf<UInventoryHUDWidget> InventoryHUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Inventory")
	UInventoryManager* InventoryManagerRef;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Inventory")
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void OnOpenInventory();
	virtual void OnOpenInventory_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void OnUse();
	virtual void OnUse_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void OnStopUse();
	virtual void OnStopUse_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void StartCrouch();
	virtual void StartCrouch_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void StopCrouching();
	virtual void StopCrouching_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void StartSprint();
	virtual void StartSprint_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void StopSprinting();
	virtual void StopSprinting_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
	void OnScrollUp();
	virtual void OnScrollUp_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Control")
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
