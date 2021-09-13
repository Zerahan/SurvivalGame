// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrolNode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Engine/Texture2D.h"
#include "Components/CapsuleComponent.h"
#include "Components/BillboardComponent.h"

APatrolNode::APatrolNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 92.0f);
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume(false);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> PatrolPathTextureObject;
			FName ID_PatrolPath;
			FText NAME_PatrolPath;
			FName ID_Navigation;
			FText NAME_Navigation;
			FConstructorStatics()
				: PatrolPathTextureObject(TEXT("/Engine/EditorResources/S_NavP"))
				, ID_PatrolPath(TEXT("PatrolPath"))
				, NAME_PatrolPath(NSLOCTEXT("SpriteCategory", "PatrolPath", "Patrol Path"))
				, ID_Navigation(TEXT("Navigation"))
				, NAME_Navigation(NSLOCTEXT("SpriteCategory", "Navigation", "Navigation"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		if (GetGoodSprite())
		{
			GetGoodSprite()->Sprite = ConstructorStatics.PatrolPathTextureObject.Get();
			GetGoodSprite()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			GetGoodSprite()->SpriteInfo.Category = ConstructorStatics.ID_PatrolPath;
			GetGoodSprite()->SpriteInfo.DisplayName = ConstructorStatics.NAME_PatrolPath;
			//GetGoodSprite()->EditorScale = 8;
		}
		if (GetBadSprite())
		{
			GetBadSprite()->SetVisibility(false);
		}

		if (ArrowComponent)
		{
			ArrowComponent->ArrowColor = FColor(150, 200, 255);

			ArrowComponent->ArrowSize = 1.0f;
			ArrowComponent->bTreatAsASprite = true;
			ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Navigation;
			ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Navigation;
			ArrowComponent->SetupAttachment(GetCapsuleComponent());
			ArrowComponent->bIsScreenSizeScaled = true;
		}
	}
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITORONLY_DATA
/** Returns ArrowComponent subobject **/
UArrowComponent* APatrolNode::GetArrowComponent() const { return ArrowComponent; }
#endif

bool APatrolNode::IsClaimed(AActor* AskingActorRef) const
{
	return IsValid(OccupyingEntityRef);
}

bool APatrolNode::CanClaimNode(AActor* ClaimingActorRef) const
{
	return !IsValid(OccupyingEntityRef);
}

bool APatrolNode::ClaimNode(AActor* ClaimingActorRef)
{
	if (CanClaimNode(ClaimingActorRef)) {
		OccupyingEntityRef = ClaimingActorRef;
		return true;
	}
	return false;
}

bool APatrolNode::LeaveNode(AActor* LeavingActorRef)
{
	if (OccupyingEntityRef == LeavingActorRef) {
		OccupyingEntityRef = nullptr;
		return true;
	}
	return false;
}
