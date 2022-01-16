// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Computers/FakeApp_Commandline.h"
#include "Props/AccessibleComputer_Basic.h"
#include "Misc/OutputDeviceNull.h"

UFakeCommandHistory::UFakeCommandHistory()
	:Super()
{
	DisplayLines = FString("");
	Lines.SetNum(0);
	NewLineIndex = 0;
	HistorySize = 50;
}

void UFakeCommandHistory::SetHistorySize(int32 _HistorySize) {
	HistorySize = (uint8)FMath::Clamp(_HistorySize, 5, 250);
}

void UFakeCommandHistory::AddLine(FString NewLine, int32 NumTabs) {
	uint8 NewTabsActual = (uint8)(FMath::Clamp(NumTabs, 0, 250));
	FString Tabs = "";
	if (NewTabsActual > 0) {
		for (uint8 _ = 0; _ < NewTabsActual; _++) {
			Tabs += "\t";
		}
	}
	TArray<FString> AllLines;
	if (NewLine.IsEmpty()) {
		AllLines.Emplace(FString(""));
	}
	else {
		NewLine.ParseIntoArray(AllLines, TEXT("\n"), true);
	}
	if (AllLines.Num() > 0) {
		for (int32 lineindex = 0; lineindex < AllLines.Num(); lineindex++) {
			if (Lines.Num() < HistorySize) {
				Lines.Emplace(Tabs + AllLines[lineindex]);
				NewLineIndex = (NewLineIndex + 1) % HistorySize;
			}
			else {
				Lines[NewLineIndex] = Tabs + AllLines[lineindex];
				NewLineIndex = (NewLineIndex + 1) % HistorySize;
				//Lines[NewLineIndex].Empty();
			}
		}
		CompileLines();
	}
}

inline FString UFakeCommandHistory::GetHistory() const {
	return DisplayLines;
}

void UFakeCommandHistory::Clear() {
	Lines.Empty();
	DisplayLines = FString("");
	NewLineIndex = 0;
}

void UFakeCommandHistory::CompileLines() {
	uint8 CurrentLine = 0;
	DisplayLines = FString("");
	if (Lines.Num() > 0) {
		for (uint8 i = 0; i < Lines.Num(); i++) {
			if (NewLineIndex + i < Lines.Num()) {
				CurrentLine = NewLineIndex + i;
			}
			else {
				CurrentLine = NewLineIndex + i - Lines.Num();
			}
			if (Lines.IsValidIndex(CurrentLine)) {
				DisplayLines += Lines[CurrentLine] + "\n";
			}
		}
	}
}



/////////
//CLASS//
/////////

UFakeApp_Commandline::UFakeApp_Commandline()
{
	RecentCommandIndex = 0;
	AppName = "CMD";

	CommandHistory = CreateDefaultSubobject<UFakeCommandHistory>(TEXT("CommandHistory"));

	AddCommand("help", this, FName("Command_Help"), "Lists available commands and can get additional information about a specific command.", "help returns a list of available commands.\nhelp [specific command] returns detailed information about a command.", 0);
	AddCommand("clr", this, FName("Command_Clear"), "Clears the console", "Clears the console", 0);
	AddCommand("login", this, FName("Command_Login"), "Logs in a user.", "[username] [password] Logs in to the computer.", 0);
	AddCommand("logout", this, FName("Command_Logout"), "Logs out the current user.", "Logs out the current user.", 0);
	AddCommand("user", this, FName("Command_User"), "Gets the current user.", "Gets the current user.", 0);
	AddCommand("app", this, FName("Command_App"), "App-related commands", "App command list:\n\t[list] Print a list of accessible apps.\n\t[Open] [AppID] opens a specific app.", 0);
}

void UFakeApp_Commandline::ProcessCommand(FString Command)
{
	if(Command.IsEmpty()){
		OnConsoleUpdatedDispatcher.Broadcast();
		return;
	}
	if (RecentCommandList.Num() < 4) {
		RecentCommandList.Add(Command);
	}
	else {
		RecentCommandList[RecentCommandIndex] = Command;
		RecentCommandIndex++;
		if (RecentCommandIndex >= RecentCommandList.Num()) {
			RecentCommandIndex = 0;
		}
	}
	CommandHistory->AddLine(Command);
	
	FString CommandActual;
	TArray<FString> Args;
	if (Command.ParseIntoArray(Args, TEXT(" "), true) == 0){
		OnConsoleUpdatedDispatcher.Broadcast();
		return;
	}
	CommandActual = Args[0];
	Args.RemoveAt(0);

	int32 CommandIndex = CommandList.Find(CommandActual);
	if (!CommandInfoList.IsValidIndex(CommandIndex)) {
		CommandHistory->AddLine("ERROR: Command [" + CommandActual + "] not found.");
		OnConsoleUpdatedDispatcher.Broadcast();
		return;
	}
	FFakeCommandLine CMDHelper;
	CMDHelper.BindUFunction(CommandInfoList[CommandIndex].FunctionParentObjectRef, CommandInfoList[CommandIndex].FunctionName);
	CMDHelper.ExecuteIfBound(Args);
	CMDHelper.Unbind();
	CommandHistory->AddLine("");
	OnConsoleUpdatedDispatcher.Broadcast();
}

void UFakeApp_Commandline::Command_Help(TArray<FString> Args)
{
	if (Args.Num() == 0) {
		AAccessibleComputer_Basic* ComputerRef = Cast<AAccessibleComputer_Basic>(GetOuter());
		if (IsValid(ComputerRef)) {
			FFakeUserLoginInfo UserInfo = ComputerRef->GetCurrentUserInfo();
			CommandHistory->AddLine("List of commands available to a(n) " + FFakeUserLoginInfo::GroupIDToString(UserInfo.GroupID) + " user.");
			for (int32 i = 0; i < CommandInfoList.Num(); i++) {
				if (UserInfo.IsUserInGroup(CommandInfoList[i].RequiredGroupID)) {
					CommandHistory->AddLine(CommandInfoList[i].AliasName + " - " + CommandInfoList[i].ShortDescription, 1);
				}
			}
		}
	}
	else {
		int32 CommandIndex = CommandList.Find(Args[0]);
		CommandHistory->AddLine(CommandInfoList[CommandIndex].AliasName + " - " + CommandInfoList[CommandIndex].FullDescription, 1);
	}
}

void UFakeApp_Commandline::Command_Clear(TArray<FString> Args)
{
	CommandHistory->Clear();
	CommandHistory->AddLine("Cleared!");
}

void UFakeApp_Commandline::Command_Login(TArray<FString> Args)
{
	if (Args.Num() < 2) {
		TArray<FString> NewArgs;
		NewArgs.Insert("login", 0);
		Command_Help(NewArgs);
		return;
	}
	AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
	if (IsValid(Computer)) {
		FFakeUserLoginInfo UserLoginInfo;
		UserLoginInfo.Name		= Args[0];
		UserLoginInfo.Password	= Args[1];
		FString ConsoleMessage;
		Computer->UserLogin(UserLoginInfo, ConsoleMessage);
		CommandHistory->AddLine(ConsoleMessage);
	}
}

void UFakeApp_Commandline::Command_Logout(TArray<FString> Args)
{
	AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
	if (IsValid(Computer)) {
		FString ConsoleMessage;
		Computer->UserLogout(ConsoleMessage);
		CommandHistory->AddLine(ConsoleMessage);
	}
}

void UFakeApp_Commandline::Command_User(TArray<FString> Args)
{
	AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
	if (IsValid(Computer)) {
		FFakeUserLoginInfo UserInfo = Computer->GetCurrentUserInfo();
		if (UserInfo.IsValid()) {
			CommandHistory->AddLine("User [" + UserInfo.Name + "] is currently logged in. ID: " + FFakeUserLoginInfo::GroupIDToString(UserInfo.GroupID));
		}
		else {
			CommandHistory->AddLine("No user is logged in.");
		}
	}
}

void UFakeApp_Commandline::Command_App(TArray<FString> Args)
{
	if (Args.Num() < 1) {
		TArray<FString> NewArgs = Args;
		NewArgs.Insert("app", 0);
		Command_Help(NewArgs);
		return;
	}
	FString subcommand = Args[0].ToLower();
	if (subcommand == "list") {
		AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
		if (IsValid(Computer)) {
			FString CMD;
			TArray<int32> AppListIDs = Computer->GetAppListIDs(CMD);
			if (AppListIDs.Num() == 0) {
				CommandHistory->AddLine("No installed applications", 1);
				return;
			}
			CommandHistory->AddLine("List of installed applications: ", 1);
			for (int32 i = 0; i < AppListIDs.Num(); i++) {
				UFakeComputerApplication_Basic* App = Computer->GetAppFromID(i);
				if (IsValid(App)) {
					CommandHistory->AddLine(FString::FromInt(i) + ": " + App->GetDisplayName().ToString(), 2);
				}
				else {
					CommandHistory->AddLine(FString::FromInt(i) + ": ERROR_UNKNOWN", 2);
				}
			}
			//CommandHistory->AddLine(ConsoleMessage);
		}
		return;
	}
	if (subcommand == "open") {
		if (!Args.IsValidIndex(1)) {
			CommandHistory->AddLine("ERROR: Missing APPID", 1);
			return;
		}
		AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
		FString AppString = Args[1];
		int32 AppNumber = FCString::Atoi(*AppString);
		FString ConsoleMessage;
		Computer->OpenApp(AppNumber, ConsoleMessage);
		CommandHistory->AddLine(ConsoleMessage);
		return;
	}
	TArray<FString> NewArgs = Args;
	NewArgs.Insert("app", 0);
	Command_Help(NewArgs);
}

void UFakeApp_Commandline::AddCommand(const FString cmd, UObject* ObjectRef, const FName func, const FString desc, const FString fulldesc, uint8 groupid)
{
	CommandList.Add(cmd);
	FCommandLineEntry NewEntry = FCommandLineEntry(cmd, desc, fulldesc, ObjectRef, func, groupid);
	CommandInfoList.Add(NewEntry);
}