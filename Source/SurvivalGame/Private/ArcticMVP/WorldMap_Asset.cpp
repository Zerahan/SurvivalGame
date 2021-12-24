// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcticMVP/WorldMap_Asset.h"

void UWorldMap_Asset::GenerateMap()
{
	for (int i = 0; i < PointsOfInterest.Num(); i++) {
		if (PointsOfInterest[i].MapName.IsEmpty()) {
			PointsOfInterest[i].MapName = FText::FromString(PointsOfInterest[i].MapLoadString);
		}
		if (i > 0) {
			PointsOfInterest[i].Coordinates = FMath::RandPointInCircle(2.f);
		}
	}
}

void UWorldMap_Asset::GenerateMapFromPOIList(const TArray<FWorldMap_PointOfInterest> NewPointsOfInterest)
{
	PointsOfInterest = NewPointsOfInterest;
	GenerateMap();
}
