#pragma once
#include "VoxImport/F3DArrayRef.h"

#include "FGameSave.generated.h"

USTRUCT()
struct FGameSave
{
  GENERATED_BODY()

  UPROPERTY()
  int64 Seed;
  UPROPERTY()
  F3DArrayRef Changes;
  UPROPERTY()
  FVector PlayerPosition;
};
