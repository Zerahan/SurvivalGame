// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//static_assert(TIsArrayOrRefOfType<decltype(str), TCHAR>::Value, "Formatting string must be a TCHAR array."); \

#ifndef Debug
#define Debug(str, ...) \
{ \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(str), __VA_ARGS__)); \
	UE_LOG(LogTemp, Warning, TEXT(str), __VA_ARGS__); \
}
#endif