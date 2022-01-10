// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "Characters/SurvivalAIPawn.h"
#include "SecurityCamera_Basic.generated.h"

class UCameraSceneCaptureComponent2D;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API ASecurityCamera_Basic : public ASurvivalAIPawn
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Mesh", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraMount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Mesh", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraLookOverride;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CameraMesh;

#ifdef CLIENT
protected:
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Camera", meta = (BlueprintProtected))
	UCameraSceneCaptureComponent2D* CaptureComponent;
#endif

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Default|Camera", meta = (BlueprintProtected))
	AActor* ViewOwnerOverride;

public:
	ASecurityCamera_Basic();

	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable, Category = "Default|Camera", Client, Reliable)
	void ActivateSceneCapture(UTextureRenderTarget2D* NewRenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Default|Camera", Client, Reliable)
	void DeactivateSceneCapture();

	UFUNCTION(BlueprintCallable, Category = "Default|Camera", Client, Reliable)
	void SetViewOwnerOverride(AActor* NewOwner);
};
