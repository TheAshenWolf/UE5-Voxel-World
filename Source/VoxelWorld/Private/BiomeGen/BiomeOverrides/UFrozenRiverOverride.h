#pragma once
#include "UConfig.h"
#include "URiverOverride.h"
#include "BiomeGen/UAbsBiomeRenderer.h"

class UFrozenRiverOverride : public URiverOverride
{
public:
  static FVoxelRef GetWaterBlocks(int32 zHeight);
};

inline FVoxelRef UFrozenRiverOverride::GetWaterBlocks(int32 zHeight)
{
  if (zHeight == UConfig::WATER_HEIGHT)
  {
    return FVoxelRef(false, (int32)EBasicBlock::Ice);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Water);
}