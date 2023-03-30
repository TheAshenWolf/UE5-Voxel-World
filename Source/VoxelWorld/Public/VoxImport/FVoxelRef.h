#pragma once

#include "FVoxelRef.generated.h"

USTRUCT()
struct FVoxelRef
{
  GENERATED_BODY()
  /// <summary>
  /// Whether the voxel is empty or has a color
  /// </summary>
  UPROPERTY() bool IsEmpty;
  /// <summary>
  /// Color of the voxel; ignored if IsEmpty is true
  /// </summary>
  UPROPERTY() int32 Color;
  
  bool IsEmptyOrWater(bool amIWater);
  FVoxelRef()
    : IsEmpty(true)
      , Color(0)
  {
  }

  FVoxelRef(bool isEmpty, int32 color)
    : IsEmpty(isEmpty)
      , Color(color)
  {
  }
};