// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/BasicWeaponData.h"

inline void UBasicWeaponData::SetAmmoCount(const int32 NewAmmoCount) { AmmoCount = NewAmmoCount; }

inline int32 UBasicWeaponData::GetAmmoCount() const { return AmmoCount; }
