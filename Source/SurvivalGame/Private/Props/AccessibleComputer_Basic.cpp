// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/AccessibleComputer_Basic.h"
#include "Props/Computers/FakeComputerApplication_Basic.h"
#include "Props/Computers/FakeApp_Commandline.h"

// Sets default values
AAccessibleComputer_Basic::AAccessibleComputer_Basic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//InstalledAppList.Add(CreateDefaultSubobject<UFakeApp_Commandline>(TEXT("OS")));
	IdleTimeBeforeAutoLogout = 30;
	OperatingSystemClass = UFakeApp_Commandline::StaticClass();
	InstalledAppList.Empty();
	UserNames.Empty();
	UserPasswords.Empty();
	UserGroupIDs.Empty();
	//InstalledAppList.SetNum(1);
}

// Called when the game starts or when spawned
void AAccessibleComputer_Basic::BeginPlay()
{
	UFakeApp_Commandline* App = NewObject<UFakeApp_Commandline>(this, OperatingSystemClass, FName("OperatingSystem"));
	FString ConsoleMessage;
	InstalledAppList.Empty();
	uint8 RequiredGroupID = FFakeUserLoginInfo::BuildUserGroupID(EFakeUserPrivilages::Basic, EFakeUserPrivilages::Standard);
	InstallAppFromClass(OperatingSystemClass, RequiredGroupID, ConsoleMessage);
	UserAdd("admin", "admin", FFakeUserLoginInfo::BuildUserGroupID(EFakeUserPrivilages::Expert, EFakeUserPrivilages::Admin), ConsoleMessage);
	/*
	if (IsValid(App)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "OS Is Valid");
		UE_LOG(LogTemp, Warning, TEXT("OS Is Valid"));
	}
	//*/
	//InstalledAppList.SetNum(1);
	//InstalledAppList[0] = App;
	//int32 Index = InstalledAppList.Add(App);
	/*
	if (IsValid(InstalledAppList[0])) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "List Is Valid");
		UE_LOG(LogTemp, Warning, TEXT("List Is Valid"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Invalid list: " + FString::FromInt(InstalledAppList.Num()));
		UE_LOG(LogTemp, Warning, TEXT("Invalid list: %i"), InstalledAppList.Num());
	}
	//*/
	Super::BeginPlay();
}

// Called every frame
void AAccessibleComputer_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAccessibleComputer_Basic::SetAutoLogoutTime(int32 TimeInMinutes)
{
	IdleTimeBeforeAutoLogout = TimeInMinutes;
	if (IdleTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
	}
	ResetIdleTimer();
}

inline FFakeUserLoginInfo AAccessibleComputer_Basic::GetCurrentUserInfo() const { return CurrentUser; }

void AAccessibleComputer_Basic::ResetIdleTimer()
{
	if(IdleTimeBeforeAutoLogout <= 0) return;
	if (ActiveControllers.Num() > 0) {
		GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
	}
	else {
		GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &AAccessibleComputer_Basic::LogoutCurrentUser, IdleTimeBeforeAutoLogout);
	}
}

inline UFakeComputerApplication_Basic* AAccessibleComputer_Basic::GetAppFromID(int32 AppID) const {
	if (!InstalledAppList.IsValidIndex(AppID)){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AppID Is Valid");
		UE_LOG(LogTemp, Warning, TEXT("AppID Is Valid"));
		return nullptr;
	}
	return InstalledAppList[AppID];
}

void AAccessibleComputer_Basic::UserLogin_Implementation(FFakeUserLoginInfo LoginInfo, FString& ConsoleMessage)
{
	if (CurrentUser.IsValid()) {
		ConsoleMessage += "\tERROR: A user is already logged in. You must first log out to switch to a different user.\n";
		return;
	}
	if (IsUserInfoVerified(LoginInfo, ConsoleMessage)) {
		ConsoleMessage += "\tLogged in as user [" + LoginInfo.Name + "].";
		CurrentUser = LoginInfo;
	}
}

void AAccessibleComputer_Basic::UserLogout_Implementation(FString& ConsoleMessage)
{
	if (!CurrentUser.IsValid()) {
		ConsoleMessage += "\tERROR: Unable to log out. You must log in to log out.\n";
		return;
	}
	ConsoleMessage += "\tUser [" + CurrentUser.Name + "] has been logged out.";
	LogoutCurrentUser();
}

void AAccessibleComputer_Basic::UserAdd_Implementation(const FString& UserName, const FString& Password, const uint8 GroupID, FString& ConsoleMessage)
{
	int32 index = UserNames.Find(UserName);
	if (index != INDEX_NONE) {
		if (!Password.IsEmpty()) {
			UserPasswords[index] = Password;
		}
		if (GroupID != 0) {
			UserGroupIDs[index] = GroupID;
		}
	}
	else {
		UserNames.Add(UserName);
		UserPasswords.Add(Password);
		UserGroupIDs.Add(GroupID);
	}
}

bool AAccessibleComputer_Basic::IsUserInfoVerified(FFakeUserLoginInfo& LoginInfo, FString& ConsoleMessage)
{
	int32 UserIndex = UserNames.Find(LoginInfo.Name);
	if(UserIndex == INDEX_NONE){
		ConsoleMessage += "\tERROR: User [" + LoginInfo.Name + "] does not exist.";
		return false;
	}
	if(!UserNames.IsValidIndex(UserIndex) || !UserPasswords.IsValidIndex(UserIndex) || !UserGroupIDs.IsValidIndex(UserIndex)){
		ConsoleMessage += "\tERROR: Malformed User Database.";
		return false;
	}
	if (!LoginInfo.Name.Equals(UserNames[UserIndex])) {
		ConsoleMessage += "\tERROR: User [" + LoginInfo.Name + "] does not exist.";
		return false;
	}
	if (!LoginInfo.Password.Equals(UserPasswords[UserIndex])) {
		ConsoleMessage += "\tERROR: User [" + LoginInfo.Name + "] does not exist.";
		return false;
	}
	LoginInfo.GroupID = UserGroupIDs[UserIndex];
	return true;
}

bool AAccessibleComputer_Basic::UserHasAccessPrivilage(uint8 TargetAppID, FString& ConsoleMessage) const
{
	//ConsoleMessage += "\t" + FFakeUserLoginInfo::GroupIDToString(InstalledAppList[TargetAppID]->GetRequiredGroupID()) + " vs " + FFakeUserLoginInfo::GroupIDToString(CurrentUser.GroupID);
	if (!IsValidApp(TargetAppID, ConsoleMessage)) return false;
	if (!CurrentUser.IsUserInGroup(InstalledAppList[TargetAppID]->GetRequiredGroupID())) {
		//ConsoleMessage += "\tALERT: USER_CURRENT ATTEMPTED TO ACCESS RESTRICTED FILES. SECURITY HAS BEEN NOTIFIED.\n";
		return false;
	}
	return true;
}

bool AAccessibleComputer_Basic::IsValidApp(uint8 TargetAppID, FString& ConsoleMessage) const
{
	if (!InstalledAppList.IsValidIndex(TargetAppID)) {
		ConsoleMessage += "\tERROR: UNKNOWN APP_ID (" + FString::FromInt(TargetAppID) + ")\n";
		return false;
	}
	return true;
}

void AAccessibleComputer_Basic::OpenApp_Implementation(const int32 TargetAppID, FString& ConsoleMessage)
{
	if(!IsValidApp(TargetAppID, ConsoleMessage)) return;
	if (!UserHasAccessPrivilage(TargetAppID, ConsoleMessage)){
		ConsoleMessage += "\tALERT: USER_CURRENT ATTEMPTED TO ACCESS RESTRICTED FILES. SECURITY HAS BEEN NOTIFIED.\n";
		return;
	}
	if (TargetAppID == 0) {
		ConsoleMessage += "\tERROR: Cannot open AppID (" + FString::FromInt(TargetAppID) + ")\n";
		return;
	}
	ConsoleMessage += "\tOpening AppID (" + FString::FromInt(TargetAppID) + "): " + InstalledAppList[TargetAppID]->GetDisplayName().ToString();
	CurrentOpenApp = TargetAppID;
	OnOpenAppDispatcher.Broadcast(CurrentOpenApp);
}

TArray<int32> AAccessibleComputer_Basic::GetAppListIDs(FString& ConsoleMessage) const
{
	TArray<int32> CulledAppList;
	CulledAppList.Empty();
	for (int32 i = 0; i < InstalledAppList.Num(); i++) {
		FString CMD;
		if (UserHasAccessPrivilage(i, CMD)) {
			CulledAppList.Add(i);
		}
	}
	return CulledAppList;
}

void AAccessibleComputer_Basic::CloseApp_Implementation(FString& ConsoleMessage)
{
	OnCloseAppDispatcher.Broadcast(CurrentOpenApp);
}

void AAccessibleComputer_Basic::InstallAppFromClass_Implementation(const TSubclassOf<UFakeComputerApplication_Basic> AppClass, const uint8 RequiredGroupID, FString& ConsoleMessage)
{
	UFakeApp_Commandline* App = NewObject<UFakeApp_Commandline>(this, AppClass, AppClass.Get()->GetFName());
	App->AssignGroupID(RequiredGroupID);
	InstalledAppList.Add(App);
}

void AAccessibleComputer_Basic::InstallApp_Implementation(UFakeComputerApplication_Basic* AppRef, FString& ConsoleMessage)
{
	InstalledAppList.Add(AppRef);
}

void AAccessibleComputer_Basic::LogoutCurrentUser()
{
	if (CurrentOpenApp != 0) {
		FString Error;
		CloseApp(Error);
	}
	CurrentUser = FFakeUserLoginInfo();
}

bool AAccessibleComputer_Basic::CanInteract_Implementation(const FInteractionInfo Info) const
{
	return ActiveControllers.Num() == 0;
}

void AAccessibleComputer_Basic::BeginInteraction_Implementation(const FInteractionInfo Info)
{
	ActiveControllers.AddUnique(Info.ControllerRef);
	ResetIdleTimer();
}

void AAccessibleComputer_Basic::EndInteraction_Implementation(const FInteractionInfo Info)
{
	ActiveControllers.Remove(Info.ControllerRef);
	ResetIdleTimer();
}

void AAccessibleComputer_Basic::CancelInteraction_Implementation(const FInteractionInfo Info)
{
	ActiveControllers.Remove(Info.ControllerRef);
	ResetIdleTimer();
}

