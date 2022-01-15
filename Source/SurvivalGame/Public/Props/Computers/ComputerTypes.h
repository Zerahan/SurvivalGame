// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComputerTypes.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EFakeUserPrivilages : uint8
{
	None		= 0		 UMETA(Hidden),
	Basic		= 1 << 0 UMETA(DisplayName = "Basic"), // 1
	Advanced	= 1 << 1 UMETA(DisplayName = "Advanced"), // 2
	Expert		= 1 << 2 UMETA(DisplayName = "Expert"), // 4
	Standard	= 1 << 3 UMETA(DisplayName = "General"), // 8
	Security	= 1 << 4 UMETA(DisplayName = "Security"), // 16
	Admin		= 1 << 5 UMETA(DisplayName = "Administrator"), // 32
};
ENUM_CLASS_FLAGS(EFakeUserPrivilages);

USTRUCT(BlueprintType)
struct SURVIVALGAME_API FFakeUserLoginInfo {
	GENERATED_BODY()

public:
	FFakeUserLoginInfo(const FString _Name = "NULL", const FString _Password = "NULL", const uint8 _AccessLevel = 0)
	{
		Name		= _Name;
		Password	= _Password;
		GroupID		= _AccessLevel;
	};

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Computer|Users")
	FString Name;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Computer|Users")
	FString Password;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default|Computer|Users")
	uint8 GroupID;

	bool IsValid() const {
		return Name != "NULL";
	}
	
	bool IsUserInGroup(uint8 UserGroup) const {
		return EnumHasAllFlags<EFakeUserPrivilages>(EFakeUserPrivilages(GroupID), EFakeUserPrivilages(UserGroup));
	}
	
	static uint8 BuildUserGroupID(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group) {
		uint8 NewGroupID = 0;
		switch (Level) {
			case EFakeUserPrivilages::Expert:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Expert;
				//[[fallthrough]];
			case EFakeUserPrivilages::Advanced:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Advanced;
				//[[fallthrough]];
			case EFakeUserPrivilages::Basic:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Basic;
				break;
		}
		switch (Group) {
			case EFakeUserPrivilages::Standard:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Standard;
				break;
			case EFakeUserPrivilages::Security:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Security;
				break;
			case EFakeUserPrivilages::Admin:
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Admin;
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Security;
				NewGroupID = NewGroupID | (uint8)EFakeUserPrivilages::Standard;
				break;
		}
		return NewGroupID;
	}

	void SetUserGroup(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group) {
		GroupID = FFakeUserLoginInfo::BuildUserGroupID(Level, Group);
	}

	static FString GroupIDToString(const uint8 GroupID) {
		FString str;
		if ((GroupID & (uint8)EFakeUserPrivilages::Expert) > 0) {
			str += "Expert ";
		}
		else if((GroupID & (uint8)EFakeUserPrivilages::Advanced) > 0) {
			str += "Advanced ";
		}
		else if ((GroupID & (uint8)EFakeUserPrivilages::Basic) > 0) {
			str += "Basic ";
		}
		else {
			str += "New ";
		}
		if ((GroupID & (uint8)EFakeUserPrivilages::Admin) > 0) {
			str += "Admin";
		}
		else if ((GroupID & (uint8)EFakeUserPrivilages::Security) > 0) {
			str += "Security";
		}
		else if ((GroupID & (uint8)EFakeUserPrivilages::Standard) > 0) {
			str += "Standard";
		}
		else {
			str += "Unknown";
		}
		return str;
	}
};