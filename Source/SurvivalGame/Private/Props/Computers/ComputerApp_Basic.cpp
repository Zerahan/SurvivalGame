// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/Computers/ComputerApp_Basic.h"
#include "UI/ComputerAppWidget_Basic.h"

UComputerApp_Basic::UComputerApp_Basic()
{
	WidgetClass = UComputerAppWidget_Basic::StaticClass();
	RequiredGroupID = 0;
}

inline uint8 UComputerApp_Basic::GetRequiredGroupID() const { return RequiredGroupID; }

inline FName UComputerApp_Basic::GetDisplayName() const { return AppName; }

void UComputerApp_Basic::AssignGroupID(const EFakeUserPrivilages Level, const EFakeUserPrivilages Group)
{
	RequiredGroupID = FFakeUserLoginInfo::BuildUserGroupID(Level, Group);
}

inline TSubclassOf<UComputerAppWidget_Basic> UComputerApp_Basic::GetWidgetClass() const { return WidgetClass;}

UUserWidget* UComputerApp_Basic::GenerateWidget(APlayerController* Controller)
{
	UComputerAppWidget_Basic* UserWidget = CreateWidget<UComputerAppWidget_Basic>(Controller, WidgetClass, "ComputerAppWidget");
	if (IsValid(UserWidget)) {
		UserWidget->SetAppRef(this);
	}
	return UserWidget;
}

UComputerApp_Basic* UComputerApp_Basic::CopyApp()
{
	return DuplicateObject(this, GetOuter());
}
