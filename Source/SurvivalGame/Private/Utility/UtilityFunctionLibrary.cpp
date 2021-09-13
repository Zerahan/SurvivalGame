// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/UtilityFunctionLibrary.h"
#include "Input/Reply.h"

bool UUtilityFunctionLibrary::IsEventReplyHandled(FEventReply Reply)
{
	return Reply.NativeReply.IsEventHandled();
}

void UUtilityFunctionLibrary::PrintDebug(const FString& str, const FColor Color)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, str);
}

int UUtilityFunctionLibrary::CompareString(const FString& StringA, const FString& StringB, const bool IsDescendingOrder)
{
	if (IsDescendingOrder) {
		return StringA.Compare(StringB,ESearchCase::IgnoreCase);
	}
	else {
		return StringB.Compare(StringA,ESearchCase::IgnoreCase);
	}
}
