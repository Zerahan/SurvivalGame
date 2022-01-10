// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CameraSceneCaptureComponent2D.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UCameraSceneCaptureComponent2D : public USceneCaptureComponent2D
{
	GENERATED_BODY()

	virtual const AActor* GetViewOwner() const override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* ViewOwner;

	UFUNCTION()
	void SetViewOwner(AActor* NewViewOwner);
};
