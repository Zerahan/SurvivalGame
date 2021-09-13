// Fill out your copyright notice in the Description page of Project Settings.


#include "DEPRECATED/ItemData_SingleInt.h"

inline int32 UItemData_SingleInt::GetStat() const						{ return Stat1; }
inline void  UItemData_SingleInt::SetStat(const int32 NewStat)			{ Stat1 = NewStat; }
inline void  UItemData_SingleInt::ModStat(const int32 ModAmount)		{ SetStat(Stat1 + ModAmount); }

inline int32 UItemData_SingleInt::GetStatMAX() const					{ return Stat1_MAX; }
inline void  UItemData_SingleInt::SetStatMAX(const int32 NewStatMAX)	{ Stat1_MAX = NewStatMAX; SetStat(Stat1); }
