// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CameraSceneCaptureComponent2D.h"

inline const AActor* UCameraSceneCaptureComponent2D::GetViewOwner() const {	return ViewOwner; }

inline void UCameraSceneCaptureComponent2D::SetViewOwner(AActor* NewViewOwner) {
	if(ViewOwner == NewViewOwner) return;
	ViewOwner = NewViewOwner;
}
