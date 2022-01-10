// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "Engine/DataAsset.h"
#include "WorldMap_Asset.generated.h"


USTRUCT(BlueprintType)
struct SURVIVALGAME_API FWorldMap_PointOfInterest {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap|PointOfInterest")
	TSoftObjectPtr<class UTexture> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap|PointOfInterest")
	FText MapName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap|PointOfInterest")
	FString MapLoadString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap|PointOfInterest")
	FVector2D Coordinates;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SURVIVALGAME_API UWorldMap_Asset : public UPrimaryDataAsset
{
	GENERATED_BODY()
		
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap")
	TSoftObjectPtr<UMaterial> MapMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WorldMap")
	TArray<FWorldMap_PointOfInterest> PointsOfInterest;

	UFUNCTION(BlueprintCallable, Category = "WorldMap")
	void GenerateMap();

	UFUNCTION(BlueprintCallable, Category = "WorldMap")
	void GenerateMapFromPOIList(const TArray<FWorldMap_PointOfInterest> NewPointsOfInterest);
};
