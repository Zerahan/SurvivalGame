// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SurvivalCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/InventoryManager.h"
#include "Inventory/HotbarManager.h"
#include "Interfaces/InteractionInterface.h"
#include "UI/Inventory/InventoryHUDWidget.h"
#include "Utility/UtilityFunctionLibrary.h"
#include "DEPRECATED/BasicTool.h"

// Sets default values
ASurvivalCharacter::ASurvivalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UseDistance = 200.f;
	UseCrouchToggle = true;
	CanSprint = true;
	IsSprintHeld = false;
	IsSprinting = false;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetCapsuleComponent()->InitCapsuleSize(30.f, 96.f);
	
	CrouchedEyeHeight = BaseEyeHeight;

	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	DefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SprintSpeedMultiplier = 2.f;
	WalkSpeedMultiplier = 0.75f;

	auto* CMesh = GetMesh();
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> meshObject(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin_Female"));
	if (meshObject.Succeeded()) {
		CMesh->SetSkeletalMesh(meshObject.Object);
	}

	CMesh->SetOnlyOwnerSee(false);
	CMesh->SetOwnerNoSee(true);
	CMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CMesh->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	CMesh->SetRelativeScale3D(FVector(1.05f, 1.05f, 1.05f));
	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationObject(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP"));
	if (AnimationObject.Succeeded()) {
		CMesh->SetAnimInstanceClass(AnimationObject.Object->GeneratedClass);
	}
	
	Mesh_FirstPerson = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh_FirstPerson"));
	Mesh_FirstPerson->SetSkeletalMesh(meshObject.Object);
	Mesh_FirstPerson->SetAnimInstanceClass(AnimationObject.Object->GeneratedClass);
	Mesh_FirstPerson->SetOnlyOwnerSee(true);
	Mesh_FirstPerson->SetOwnerNoSee(false);
	Mesh_FirstPerson->SetupAttachment(GetCapsuleComponent());
	Mesh_FirstPerson->bCastDynamicShadow = false;
	Mesh_FirstPerson->CastShadow = false;
	Mesh_FirstPerson->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Mesh_FirstPerson->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	Mesh_FirstPerson->SetRelativeScale3D(FVector(1.05f, 1.05f, 1.05f));
	Mesh_FirstPerson->SetCollisionProfileName("NoCollision", false);

	CameraAttachmentComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentComponent"));
	CameraAttachmentComponent->SetupAttachment(Mesh_FirstPerson, "head");
	CameraAttachmentComponent->SetRelativeLocation(FVector(-5.f, 0.f, 0.f));
	CameraAttachmentComponent->SetRelativeRotation(FRotator(0.f, 70.f, -90.f));
	CameraAttachmentComponent->TargetArmLength = 0.f;
	CameraAttachmentComponent->bDoCollisionTest = false;
	CameraAttachmentComponent->bUsePawnControlRotation = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(CameraAttachmentComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 5.f));

	DropPointSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("DropPointSpringArm"));
	DropPointSpringArm->SetupAttachment(FirstPersonCamera);
	DropPointSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	DropPointSpringArm->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	DropPointSpringArm->TargetArmLength = 100.f;

	DropPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DropPoint"));
	DropPoint->SetupAttachment(DropPointSpringArm);

	InventoryManagerRef = CreateDefaultSubobject<UInventoryManager>(TEXT("Inventory Manager"));
	InventoryManagerRef->SetDisplayName("Backpack");
	InventoryManagerRef->SetInventorySize(8);

	HotbarManagerRef = CreateDefaultSubobject<UHotbarManager>(TEXT("Hotbar Manager"));
	HotbarManagerRef->SetDisplayName("Hotbar");
	HotbarManagerRef->SetInventorySize(4);

	//if (!InventoryHUDWidgetClass) {
		ConstructorHelpers::FClassFinder<UInventoryHUDWidget> WidgetClass (TEXT("/Game/SurvivalGame/Dev/UI/HUD_InventoryMenu")); 
		if (WidgetClass.Succeeded()) {
			InventoryHUDWidgetClass = WidgetClass.Class;
		}
		else {
			InventoryHUDWidgetClass = UInventoryHUDWidget::StaticClass();
		}
	//}

	LastInteractionInfo = FInteractionInfo();

}

// Called when the game starts or when spawned
void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* LocalControllerRef = Cast<APlayerController>(Controller);
	//UUtilityFunctionLibrary::PrintDebug("Getting controller...");

	/*
	if (LocalControllerRef) {
		//UUtilityFunctionLibrary::PrintDebug("Making widget...");
		InventoryHUDWidget = CreateWidget<UInventoryHUDWidget>(LocalControllerRef, InventoryHUDWidgetClass, "InventoryWidgetRef");
		//InventoryHUDWidget->SetOwningPlayer(LocalControllerRef);
		InventoryHUDWidget->SetInventoryRef(InventoryManagerRef);
		if (IsValid(InventoryHUDWidget)) {
			IMenuInterface* Interface = Cast<IMenuInterface>(InventoryHUDWidget);
			if (Interface) {
				Interface->Execute_Rebuild(InventoryHUDWidget);
				Interface->Execute_CloseMenu(InventoryHUDWidget);
			}
		}
		InventoryHUDWidget->AddToViewport();
	}
	//*/
	LastInteractionInfo.PawnRef = this;
}

// Called every frame
void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetWorld()->LineTraceSingleByChannel(LastLookTrace, FirstPersonCamera->GetComponentLocation(), FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 5000.f), ECollisionChannel::ECC_Camera, FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this));

	// stabilize head height
}

// Called to bind functionality to input
void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ASurvivalCharacter::OnFire);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ASurvivalCharacter::OnUse);
	PlayerInputComponent->BindAction("Use", IE_Released, this, &ASurvivalCharacter::OnStopUse);

	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ASurvivalCharacter::OnOpenInventory);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASurvivalCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASurvivalCharacter::StopCrouching);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASurvivalCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASurvivalCharacter::StopSprinting);
	
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

	PlayerInputComponent->BindAxis("CameraYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("CameraPitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ASurvivalCharacter::OnScrollUp);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ASurvivalCharacter::OnScrollDown);
}

inline bool ASurvivalCharacter::GetCanSprint() const { return CanSprint; }

void ASurvivalCharacter::SetCanSprint(bool NewCanSprint)
{
	CanSprint = NewCanSprint;
}

void ASurvivalCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!IsPendingKill())
	{
		if (Mesh_FirstPerson)
		{
			CacheInitialMeshOffset(Mesh_FirstPerson->GetRelativeLocation(), Mesh_FirstPerson->GetRelativeRotation());

			// force animation tick after movement component updates
			if (Mesh_FirstPerson->PrimaryComponentTick.bCanEverTick && GetCharacterMovement())
			{
				Mesh_FirstPerson->PrimaryComponentTick.AddPrerequisite(GetCharacterMovement(), GetCharacterMovement()->PrimaryComponentTick);
			}
			//Mesh_FirstPerson->HideBoneByName("neck_01", EPhysBodyOp::PBO_None);
		}
		if (InventoryManagerRef) {
			InventoryManagerRef->OnLinkedInventoryChangedDispatcher.AddDynamic(this, &ASurvivalCharacter::OnLinkedInventoryChanged);
		}
	}
}

inline USkeletalMeshComponent* ASurvivalCharacter::GetMesh1P() const { return Mesh_FirstPerson; }

void ASurvivalCharacter::SetInventoryHUDWidget(UInventoryHUDWidget* NewInventoryHUDWidget)
{
	InventoryHUDWidget = NewInventoryHUDWidget;
}

void ASurvivalCharacter::OnFire()
{
	/*
	if (HotbarManagerRef) {
		if (HotbarManagerRef->CurrentToolRef) {
			HotbarManagerRef->CurrentToolRef->OnPrimaryAction();
		}
	}
	//*/
}

FHitResult ASurvivalCharacter::GetLastLookTrace() const
{
	return LastLookTrace;
}

void ASurvivalCharacter::OnOpenInventory_Implementation()
{
	if (IsValid(InventoryHUDWidget)) {
		IMenuInterface* Interface = Cast<IMenuInterface>(InventoryHUDWidget);
		if (Interface) {
			Interface->Execute_ToggleMenu(InventoryHUDWidget);
		}
	}
	//UUtilityFunctionLibrary::PrintDebug(str + ">");
}

void ASurvivalCharacter::OnUse_Implementation()
{
	FHitResult Result = GetLastLookTrace();
	if (Result.bBlockingHit) {
		if (Result.Distance > UseDistance) { return; }
		AActor* HitActor = Result.GetActor();
		if (IsValid(HitActor)) {
			//UUtilityFunctionLibrary::PrintDebug("Hit: " + HitActor->GetFName().ToString());
			LastInteractionActorRef = Result.GetActor();
			LastInteractionInfo.ControllerRef = GetController();
			LastInteractionInfo.HitComponent = Result.GetComponent();
			IInteractionInterface* Interface = Cast<IInteractionInterface>(HitActor);
			if(Interface) {
				//UUtilityFunctionLibrary::PrintDebug(" - Has Interface");
				if (Interface->Execute_CanInteract(HitActor, LastInteractionInfo)) {
					Interface->Execute_BeginInteraction(HitActor, LastInteractionInfo);
				}
			}
		}
	}
}

void ASurvivalCharacter::OnStopUse_Implementation()
{
}

void ASurvivalCharacter::StartCrouch_Implementation()
{
	if(UseCrouchToggle){
		if (bIsCrouched) {
			UnCrouch();
		}
		else {
			Crouch();
		}
		return;
	}
	StopSprinting();
	Crouch();
}

void ASurvivalCharacter::StopCrouching_Implementation()
{
	if (UseCrouchToggle) { return; }
	UnCrouch();
}

void ASurvivalCharacter::StartSprint_Implementation()
{
	IsSprintHeld = true;
	TrySprinting();
}

void ASurvivalCharacter::StopSprinting_Implementation()
{
	IsSprintHeld = false;
	TrySprinting();
}

void ASurvivalCharacter::TrySprinting_Implementation()
{
	bool NewIsSprinting = CanSprint;
	if (CanSprint){
		NewIsSprinting = !(MoveInput.Y < 0) && IsSprintHeld && (MoveInput.Y > 0 || MoveInput.X != 0);
	}
	if (IsSprinting != NewIsSprinting) {
		IsSprinting = NewIsSprinting;
		if(IsSprinting){
			UnCrouch();
			GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed * SprintSpeedMultiplier;
		}else{
			GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
		}
	}
}

void ASurvivalCharacter::OnScrollUp_Implementation()
{
	if (IsValid(HotbarManagerRef)) {
		HotbarManagerRef->SelectPreviousItem();
	}
}

void ASurvivalCharacter::OnScrollDown_Implementation()
{
	if (IsValid(HotbarManagerRef)) {
		HotbarManagerRef->SelectNextItem();
	}
}

void ASurvivalCharacter::MoveForward(float AxisValue)
{
	MoveInput.Y = AxisValue;
	if (AxisValue != 0.f) {
		AddMovementInput(GetActorForwardVector(), AxisValue);
		TrySprinting();
	}
}

void ASurvivalCharacter::MoveRight(float AxisValue)
{
	MoveInput.X = AxisValue;
	if (AxisValue != 0.f) {
		AddMovementInput(GetActorRightVector(), AxisValue);
		TrySprinting();
	}
}

void ASurvivalCharacter::OnLinkedInventoryChanged()
{
	if (IsValid(InventoryManagerRef->GetLinkedInventory())) {
		OnOpenInventory();
	}
}
