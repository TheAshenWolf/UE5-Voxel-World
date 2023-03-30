#pragma once
#include "UConfig.h"
#include "UOceanOverride.h"
#include "BiomeGen/UAbsBiomeRenderer.h"

class UFrozenOceanOverride : public UOceanOverride
{
public:

   static FVoxelRef GetWaterBlocks(int32 zHeight);
};

inline FVoxelRef UFrozenOceanOverride::GetWaterBlocks(int32 zHeight)
{
  if (zHeight == UConfig::WATER_HEIGHT)
  {
    return FVoxelRef(false, (int32)EBasicBlock::Ice);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Water);
}
