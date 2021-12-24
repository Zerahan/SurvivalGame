// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcticMVP/Game/ArcticMVP_GameInstance.h"
#include "ArcticMVP/WorldMap_Asset.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"


UArcticMVP_GameInstance::UArcticMVP_GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WorldMapAssetClass = UWorldMap_Asset::StaticClass();
}

void UArcticMVP_GameInstance::Init()
{
	if (WorldMapAssetClass.IsValid()) {
		WorldMapAsset_Instance = WorldMapAssetClass.Get()->GetDefaultObject<UWorldMap_Asset>();
		WorldMapAsset_Instance.LoadSynchronous();
		WorldMapAsset_Instance.Get()->GenerateMap();
		//UGameplayStatics::OpenLevel(this, FName(WorldMapAsset_Instance.Get()->PointsOfInterest[0].MapLoadString), true);
	}
}