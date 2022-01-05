// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SecurityCamera_Basic.h"
#include "UI/CameraSceneCaptureComponent2D.h"


ASecurityCamera_Basic::ASecurityCamera_Basic()
	:Super()
{
	CameraMount = CreateDefaultSubobject<USceneComponent>(TEXT("CameraMount"));
	CameraMount->SetupAttachment(GetRootComponent());
	//CameraMount->SetRelativeLocationAndRotation(FVector(5, 0, 0), FRotator(0, 90, -90));

	CameraLookOverride = CreateDefaultSubobject<USceneComponent>(TEXT("CameraLookOverride"));
	CameraLookOverride->SetupAttachment(CameraMount);

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));
	CameraMesh->SetupAttachment(CameraLookOverride);
	CameraMesh->SetRelativeScale3D(FVector(0.75f, 1.f, 1.f));
	//CameraMesh->SetRelativeRotation(FRotator(90, 0, 90));
	CameraMesh->SetRelativeLocation(FVector(40, 0, 0));
	CameraMesh->bOwnerNoSee = true;

#ifdef CLIENT
	//if (GEngine->GetNetMode(GetWorld()) == NM_Client) {
	CaptureComponent = CreateDefaultSubobject<UCameraSceneCaptureComponent2D>(TEXT("ClientsideCaptureComponent"));
	CaptureComponent->SetupAttachment(CameraLookOverride);
	FPostProcessSettings PostProcessSettings	= FPostProcessSettings();
	CaptureComponent->PostProcessSettings		= PostProcessSettings;
	CaptureComponent->PrimitiveRenderMode		= ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	CaptureComponent->CaptureSource				= ESceneCaptureSource::SCS_FinalColorLDR;
	CaptureComponent->bCaptureEveryFrame		= true;
	CaptureComponent->bCaptureOnMovement		= false;
	CaptureComponent->FOVAngle					= 120.f;
	CaptureComponent->TextureTarget				= nullptr;
	CaptureComponent->bAutoActivate = false;
	CaptureComponent->SetActive(false);
	//}
#endif
	SetViewOwnerOverride(this);
}

void ASecurityCamera_Basic::Destroyed()
{
#ifdef CLIENT
	CaptureComponent = nullptr;
#endif
	Super::Destroyed();
}

void ASecurityCamera_Basic::ActivateSceneCapture_Implementation(UTextureRenderTarget2D* NewRenderTarget)
{
#ifdef CLIENT
	CaptureComponent->TextureTarget = NewRenderTarget;
	CaptureComponent->SetActive(true, true);
#endif
}

void ASecurityCamera_Basic::DeactivateSceneCapture_Implementation()
{
#ifdef CLIENT
	CaptureComponent->SetActive(false);
	CaptureComponent->TextureTarget = nullptr;
#endif
}

void ASecurityCamera_Basic::SetViewOwnerOverride_Implementation(AActor* NewOwner)
{
	if (ViewOwnerOverride == NewOwner) return;
	ViewOwnerOverride = NewOwner;
#ifdef CLIENT
	CaptureComponent->SetViewOwner(NewOwner);
#endif
}
