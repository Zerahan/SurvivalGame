// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Computers/FakeComputerApplication_Basic.h"
#include "UI/FakeComputerApplicationWidget_Basic.h"
#include "Props/Computers/FakeComputerApplication_Basic.h"
#include "Props/AccessibleComputer_Basic.h"

UFakeComputerApplication_Basic::UFakeComputerApplication_Basic()
{
	WidgetClass = UFakeComputerApplicationWidget_Basic::StaticClass();
	RequiredGroupID = 0;
}

void UFakeComputerApplication_Basic::Initialize_Implementation()
{
}

inline uint8 UFakeComputerApplication_Basic::GetRequiredGroupID() const { return RequiredGroupID; }

inline FName UFakeComputerApplication_Basic::GetDisplayName() const { return AppName; }

inline AAccessibleComputer_Basic* UFakeComputerApplication_Basic::GetOwningComputer() const { return Cast<AAccessibleComputer_Basic>(GetOuter()); }

void UFakeComputerApplication_Basic::AssignGroupIDFromPrivilages(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group)
{
	AssignGroupID(FFakeUserLoginInfo::BuildUserGroupID(Level, Group));
}

void UFakeComputerApplication_Basic::AssignGroupID(const uint8 GroupID)
{
	RequiredGroupID = GroupID;
}

inline TSubclassOf<UFakeComputerApplicationWidget_Basic> UFakeComputerApplication_Basic::GetWidgetClass() const { return WidgetClass;}

UUserWidget* UFakeComputerApplication_Basic::GenerateWidget(APlayerController* Controller)
{
	UFakeComputerApplicationWidget_Basic* UserWidget = CreateWidget<UFakeComputerApplicationWidget_Basic>(Controller, WidgetClass, "ComputerAppWidget");
	if (IsValid(UserWidget)) {
		UserWidget->SetAppRef(this);
	}
	return UserWidget;
}

UFakeComputerApplication_Basic* UFakeComputerApplication_Basic::CopyApp()
{
	return DuplicateObject(this, GetOuter());
}
