// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ArcticMVP_GameInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SURVIVALGAME_API UArcticMVP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UArcticMVP_GameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap")
	TAssetSubclassOf<class UWorldMap_Asset> WorldMapAssetClass;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap")
	TAssetPtr<class UWorldMap_Asset> WorldMapAsset_Instance;
};
