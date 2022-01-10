// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Basic.generated.h"


/**
 *
 */
UCLASS(BlueprintType, Blueprintable, Abstract, ClassGroup = (Custom))
class INVENTORYSYSTEM_API AItem_Basic : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	AItem_Basic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
