#pragma once
#include "UConfig.h"

class UColdBeachOverride
{
  public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS  = TArray {
    EBasicBlock::Snow,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sandstone,
  };
  
  static FVoxelRef GetWaterBlocks(int32 zHeight);
};

inline FVoxelRef UColdBeachOverride::GetWaterBlocks(int32 zHeight)
{
  if (zHeight == UConfig::WATER_HEIGHT)
  {
    return FVoxelRef(false, (int32)EBasicBlock::Ice);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Water);
}