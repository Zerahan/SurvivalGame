// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/Computers/FakeComputerApplication_Basic.h"
#include "FakeApp_Commandline.generated.h"

DECLARE_DELEGATE_OneParam(FFakeCommandLine, TArray<FString>);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnConsoleUpdatedDispatcher, UFakeApp_Commandline, OnConsoleUpdatedDispatcher);

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FCommandLineEntry {
	GENERATED_BODY()
public:
	FCommandLineEntry(FString _AliasName = "", FString _ShortDescription = "", FString _FullDescription = "", UObject* _FunctionParentObjectRef = nullptr, FName _FunctionName = NAME_None, uint8 _RequiredGroupID = 0)
	{
		AliasName				= _AliasName;
		ShortDescription		= _ShortDescription;
		FullDescription			= _FullDescription;
		FunctionParentObjectRef = _FunctionParentObjectRef;
		FunctionName			= _FunctionName;
		RequiredGroupID			= _RequiredGroupID;
	}

	UPROPERTY(BlueprintReadOnly)
	FString AliasName;
	
	UPROPERTY(BlueprintReadOnly)
	FString ShortDescription;

	UPROPERTY(BlueprintReadOnly)
	FString FullDescription;

	UPROPERTY(BlueprintReadOnly)
	UObject* FunctionParentObjectRef;

	UPROPERTY(BlueprintReadOnly)
	FName FunctionName;

	UPROPERTY(BlueprintReadOnly)
	uint8 RequiredGroupID;
};

UCLASS(BlueprintType)
class SURVIVALGAME_API UFakeCommandHistory : public UObject {
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> Lines;

	UPROPERTY()
	FString DisplayLines;

	UPROPERTY()
	uint8 NewLineIndex;

	UPROPERTY()
	uint8 HistorySize;

public:
	UFakeCommandHistory();

	UFUNCTION(BlueprintCallable)
	void SetHistorySize(int32 _HistorySize);

	UFUNCTION(BlueprintCallable)
	void AddLine(FString NewLine, int32 NumTabs = 0);

	UFUNCTION(BlueprintCallable)
	FString GetHistory() const;

	UFUNCTION(BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintCallable)
	void CompileLines();
};

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom))
class SURVIVALGAME_API UFakeApp_Commandline : public UFakeComputerApplication_Basic
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FString> CommandList;

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FCommandLineEntry> CommandInfoList;

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FString> CommandDescriptions;

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess="true"))
	TArray<FName> CommandFunctionList;

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FString> RecentCommandList;
	
	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	int32 RecentCommandIndex;

	UPROPERTY(BlueprintReadWrite, Category = "Default|Inventory", meta = (AllowPrivateAccess = "true"))
	UFakeCommandHistory* CommandHistory;
	
public:
	UFakeApp_Commandline();

	UFUNCTION(BlueprintCallable, Category = "Default|Computer|CMD")
	void ProcessCommand(FString Command);

	UFUNCTION(BlueprintCallable)
	void Command_Help(TArray<FString> Args);

	UFUNCTION(BlueprintCallable)
	void Command_Clear(TArray<FString> Args);

	UFUNCTION(BlueprintCallable)
	void Command_Login(TArray<FString> Args);

	UFUNCTION(BlueprintCallable)
	void Command_Logout(TArray<FString> Args);

	UFUNCTION(BlueprintCallable)
	void Command_User(TArray<FString> Args);

	UFUNCTION(BlueprintCallable)
	void Command_App(TArray<FString> Args);

	void AddCommand(const FString cmd, UObject* ObjectRef, const FName func, const FString desc, const FString fulldesc, uint8 groupid = 0);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Default|Inventory")
	FOnConsoleUpdatedDispatcher OnConsoleUpdatedDispatcher;
	//FFakeCommandLine CMDHelper;
};
