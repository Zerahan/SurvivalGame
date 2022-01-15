// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Computers/ComputerApp_Commandline.h"
#include "Props/AccessibleComputer_Basic.h"
#include "Misc/OutputDeviceNull.h"

UFakeCommandHistory::UFakeCommandHistory()
	:Super()
{
	SetHistorySize(50);
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
}

void UFakeCommandHistory::CompileLines() {
	uint8 CurrentLine = 0;
	DisplayLines = FString("");
	if (Lines.Num() > 0) {
		for (uint8 i = 0; i < Lines.Num(); i++) {
			if (NewLineIndex - i < 0) {
				CurrentLine = Lines.Num() - i;
			}
			else {
				CurrentLine = NewLineIndex - i;
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

UComputerApp_Commandline::UComputerApp_Commandline()
{
	RecentCommandIndex = 0;
	AppName = "CMD";

	CommandHistory = CreateDefaultSubobject<UFakeCommandHistory>(TEXT("CommandHistory"));

	AddCommand("help", this, FName("Command_Help"), "[specific command] returns a list of available commands or information on a specific command. Ex: 'help' or 'help toaster'", 0);
	AddCommand("clr", this, FName("Command_Clear"), "Clears the console", 0);
	AddCommand("login", this, FName("Command_Login"), "[username] [password] Logs in to the computer.", 0);
	AddCommand("logout", this, FName("Command_Logout"), "Logs out the current user.", 0);
	AddCommand("user", this, FName("Command_User"), "Gets the current user.", 0);

}

void UComputerApp_Commandline::ProcessCommand(FString Command)
{
	if(Command.IsEmpty()) return;
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
	
	FString CommandActual;
	TArray<FString> Args;
	if (Command.ParseIntoArray(Args, TEXT(" "), true) == 0) return;
	CommandActual = Args[0];
	Args.RemoveAt(0);

	int32 CommandIndex = CommandList.Find(CommandActual);
	if (!CommandInfoList.IsValidIndex(CommandIndex)) {
		CommandHistory->AddLine("ERROR: Command [] not found.");
		return;
	}
	CommandHistory->AddLine(Command);
	FFakeCommandLine CMDHelper;
	CMDHelper.BindUFunction(CommandInfoList[CommandIndex].FunctionParentObjectRef, CommandInfoList[CommandIndex].FunctionName);
	CMDHelper.ExecuteIfBound(Args);
	CMDHelper.Unbind();
	OnConsoleUpdatedDispatcher.Broadcast();
	//CommandInfoList[CommandIndex].Command.ExecuteIfBound(Args, ConsoleMessage);
}

void UComputerApp_Commandline::Command_Help(TArray<FString> Args)
{
	if (Args.Num() == 0) {
		AAccessibleComputer_Basic* ComputerRef = Cast<AAccessibleComputer_Basic>(GetOuter());
		if (IsValid(ComputerRef)) {
			FFakeUserLoginInfo UserInfo = ComputerRef->GetCurrentUserInfo();
			CommandHistory->AddLine("List of commands available to a(n) " + FFakeUserLoginInfo::GroupIDToString(UserInfo.GroupID) + " user.");
			for (int32 i = 0; i < CommandInfoList.Num(); i++) {
				if (UserInfo.IsUserInGroup(CommandInfoList[i].RequiredGroupID)) {
					CommandHistory->AddLine(CommandInfoList[i].AliasName + " - " + CommandInfoList[i].Description, 1);
				}
			}
		}
	}
	else {
		int32 CommandIndex = CommandList.Find(Args[0]);
		CommandHistory->AddLine(CommandInfoList[CommandIndex].AliasName + " - " + CommandInfoList[CommandIndex].Description, 1);
	}
}

void UComputerApp_Commandline::Command_Clear(TArray<FString> Args)
{
	CommandHistory->Clear();
	CommandHistory->AddLine("Cleared!");
}

void UComputerApp_Commandline::Command_Login(TArray<FString> Args)
{
	if (Args.Num() < 2) {
		CommandHistory->AddLine("ERROR: Command Requires both username and password.");
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

void UComputerApp_Commandline::Command_Logout(TArray<FString> Args)
{
	AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
	if (IsValid(Computer)) {
		FString ConsoleMessage;
		Computer->UserLogout(ConsoleMessage);
		CommandHistory->AddLine(ConsoleMessage);
	}
}

void UComputerApp_Commandline::Command_User(TArray<FString> Args)
{
	AAccessibleComputer_Basic* Computer = Cast<AAccessibleComputer_Basic>(GetOuter());
	if (IsValid(Computer)) {
		FFakeUserLoginInfo UserInfo = Computer->GetCurrentUserInfo();
		if (UserInfo.IsValid()) {
			CommandHistory->AddLine("User [" + UserInfo.Name + "] is currently logged in.");
		}
		else {
			CommandHistory->AddLine("No user is logged in.");
		}
	}
}

void UComputerApp_Commandline::AddCommand(const FString cmd, UObject* ObjectRef, const FName func, const FString desc, uint8 groupid)
{
	CommandList.Add(cmd);
	FCommandLineEntry NewEntry = FCommandLineEntry(cmd, desc, ObjectRef, func, groupid);
	CommandInfoList.Add(NewEntry);
}