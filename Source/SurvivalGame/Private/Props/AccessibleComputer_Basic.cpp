// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/AccessibleComputer_Basic.h"
#include "Props/Computers/ComputerApp_Basic.h"
#include "Props/Computers/ComputerApp_Commandline.h"

// Sets default values
AAccessibleComputer_Basic::AAccessibleComputer_Basic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//InstalledAppList.Add(CreateDefaultSubobject<UComputerApp_Commandline>(TEXT("OS")));
	IdleTimeBeforeAutoLogout = 30;
	OperatingSystemClass = UComputerApp_Commandline::StaticClass();
	InstalledAppList.Empty();
	//InstalledAppList.SetNum(1);
}

// Called when the game starts or when spawned
void AAccessibleComputer_Basic::BeginPlay()
{
	UComputerApp_Commandline* App = NewObject<UComputerApp_Commandline>(this, OperatingSystemClass, FName("OperatingSystem"));
	if (IsValid(App)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "OS Is Valid");
		UE_LOG(LogTemp, Warning, TEXT("OS Is Valid"));
	}
	//InstalledAppList[0] = App;
	int32 Index = InstalledAppList.Add(App);
	if (IsValid(InstalledAppList[0])) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "List Is Valid");
		UE_LOG(LogTemp, Warning, TEXT("List Is Valid"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Invalid list: " + FString::FromInt(InstalledAppList.Num()));
		UE_LOG(LogTemp, Warning, TEXT("Invalid list: %i"), InstalledAppList.Num());
	}
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

inline UComputerApp_Basic* AAccessibleComputer_Basic::GetAppFromID(int32 AppID) const {
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
		ConsoleMessage += "ERROR: A user is already logged in. You must first log out to switch to a different user.\n";
		return;
	}
	if (IsUserInfoVerified(LoginInfo, ConsoleMessage)) {
		CurrentUser = LoginInfo;
	}
}

void AAccessibleComputer_Basic::UserLogout_Implementation(FString& ConsoleMessage)
{
	if (!CurrentUser.IsValid()) {
		ConsoleMessage += "ERROR: Unable to log out. You must log in to log out.\n";
		return;
	}
	LogoutCurrentUser();
}

bool AAccessibleComputer_Basic::IsUserInfoVerified(FFakeUserLoginInfo& LoginInfo, FString& ConsoleMessage)
{
	int32 UserIndex = UserNames.Find(LoginInfo.Name);
	if(UserIndex == INDEX_NONE){
		ConsoleMessage += "ERROR: User [" + LoginInfo.Name + "] does not exist.";
		return false;
	}
	if(UserPasswords.IsValidIndex(UserIndex) || UserGroupIDs.IsValidIndex(UserIndex)){
		ConsoleMessage += "ERROR: Malformed User Database.";
		return false;
	}
	if(LoginInfo.Password != UserPasswords[UserIndex]) {
		ConsoleMessage += "ERROR: User [" + LoginInfo.Name + "] does not exist.";
		return false;
	}
	LoginInfo.GroupID = UserGroupIDs[UserIndex];
	return true;
}

bool AAccessibleComputer_Basic::UserHasAccessPrivilage(uint8 TargetAppID, FString& ConsoleMessage) const
{
	if (!IsValidApp(TargetAppID, ConsoleMessage)) return false;
	if (CurrentUser.IsUserInGroup(InstalledAppList[TargetAppID]->GetRequiredGroupID())) {
		ConsoleMessage += "ALERT: USER_CURRENT ATTEMPTED TO ACCESS RESTRICTED FILES. SECURITY HAS BEEN NOTIFIED.\n";
		return false;
	}
	return true;
}

bool AAccessibleComputer_Basic::IsValidApp(uint8 TargetAppID, FString& ConsoleMessage) const
{
	if (!InstalledAppList.IsValidIndex(TargetAppID)) {
		ConsoleMessage += "ERROR: UNKNOWN APP_ID (" + FString::FromInt(TargetAppID) + ")\n";
		return false;
	}
	return true;
}

void AAccessibleComputer_Basic::OpenApp_Implementation(const int32 TargetAppID, FString& ConsoleMessage)
{
	if(!IsValidApp(TargetAppID, ConsoleMessage)) return;
	if (!UserHasAccessPrivilage(TargetAppID, ConsoleMessage)) return;
	CurrentOpenApp = TargetAppID;
	OnOpenAppDispatcher.Broadcast(CurrentOpenApp);
}

void AAccessibleComputer_Basic::CloseApp_Implementation(FString& ConsoleMessage)
{
	OnCloseAppDispatcher.Broadcast(CurrentOpenApp);
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

