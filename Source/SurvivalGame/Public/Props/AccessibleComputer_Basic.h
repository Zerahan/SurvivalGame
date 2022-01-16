// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurvivalGame.h"
#include "GameFramework/Actor.h"
//#include "Props/Computers/FakeComputerApplication_Basic.h"
#include "Props/Computers/ComputerTypes.h"
#include "Interfaces/InteractionInterface.h"
#include "AccessibleComputer_Basic.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOpenAppDispatcher, AAccessibleComputer_Basic, OnOpenAppDispatcher, int32, TargetAppID);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FCloseAppDispatcher, AAccessibleComputer_Basic, OnCloseAppDispatcher, int32, TargetAppID);

class UFakeComputerApplication_Basic;

UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API AAccessibleComputer_Basic : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UFakeComputerApplication_Basic*> InstalledAppList;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 CurrentOpenApp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FFakeUserLoginInfo CurrentUser;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FString> UserNames;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FString> UserPasswords;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<uint8> UserGroupIDs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 IdleTimeBeforeAutoLogout;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FTimerHandle IdleTimerHandle;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<AController*> ActiveControllers;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UFakeComputerApplication_Basic> OperatingSystemClass;
	
public:	
	// Sets default values for this actor's properties
	AAccessibleComputer_Basic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void SetAutoLogoutTime(int32 TimeInMinutes);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	FFakeUserLoginInfo GetCurrentUserInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void ResetIdleTimer();

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	UFakeComputerApplication_Basic* GetAppFromID(int32 AppID) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void UserLogin(FFakeUserLoginInfo LoginInfo, FString& ConsoleMessage);
	virtual void UserLogin_Implementation(FFakeUserLoginInfo LoginInfo, FString& ConsoleMessage);

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void UserLogout(FString& ConsoleMessage);
	virtual void UserLogout_Implementation(FString& ConsoleMessage);

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void UserAdd(const FString& UserName, const FString& Password, const uint8 GroupID, FString& ConsoleMessage);
	virtual void UserAdd_Implementation(const FString& UserName, const FString& Password, const uint8 GroupID, FString& ConsoleMessage);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	bool IsUserInfoVerified(FFakeUserLoginInfo& LoginInfo, FString& ConsoleMessage);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	bool UserHasAccessPrivilage(uint8 TargetAppID, FString& ConsoleMessage) const;

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	bool IsValidApp(uint8 TargetAppID, FString& ConsoleMessage) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void OpenApp(const int32 TargetAppID, FString& ConsoleMessage);
	virtual void OpenApp_Implementation(const int32 TargetAppID, FString& ConsoleMessage);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	TArray<int32> GetAppListIDs(FString& ConsoleMessage) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void CloseApp(FString& ConsoleMessage);
	virtual void CloseApp_Implementation(FString& ConsoleMessage);

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void InstallAppFromClass(TSubclassOf<UFakeComputerApplication_Basic> AppClass, const uint8 RequiredGroupID, FString& ConsoleMessage);
	virtual void InstallAppFromClass_Implementation(TSubclassOf<UFakeComputerApplication_Basic> AppClass, const uint8 RequiredGroupID, FString& ConsoleMessage);

	UFUNCTION(BlueprintNativeEvent, Category = "Default|Computer")
	void InstallApp(UFakeComputerApplication_Basic* AppRef, FString& ConsoleMessage);
	virtual void InstallApp_Implementation(UFakeComputerApplication_Basic* AppRef, FString& ConsoleMessage);

	UFUNCTION(BlueprintCallable, Category = "Default|Computer")
	void LogoutCurrentUser();
	
	virtual bool CanInteract_Implementation(const FInteractionInfo Info) const override;
	virtual void BeginInteraction_Implementation(const FInteractionInfo Info) override;
	virtual void EndInteraction_Implementation(const FInteractionInfo Info) override;
	virtual void CancelInteraction_Implementation(const FInteractionInfo Info) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory")
	FOpenAppDispatcher OnOpenAppDispatcher;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory")
	FOpenAppDispatcher OnCloseAppDispatcher;
};
