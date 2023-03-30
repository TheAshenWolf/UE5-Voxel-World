#pragma once
#include "UConfig.h"
#include "UUtility.h"

class UMesaOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClay,
    EBasicBlock::HardenedClayOrange,
    EBasicBlock::HardenedClayBrown,
    EBasicBlock::HardenedClayRed,
    EBasicBlock::HardenedClayGray,
  };
  
  static FVoxelRef GetBlockAtDepth(int32 zHeight, int32 depth);
};

inline FVoxelRef UMesaOverride::GetBlockAtDepth(int32 zHeight, int32 depth)
{
  if (zHeight < UConfig::WORLD_HEIGHT / 3)
  {
    return FVoxelRef(false, (int32)EBasicBlock::Stone);
  }
  
  int64 seed = UUtility::GetChunkSeed(0,zHeight);
  int32 index = UUtility::NextInt(&seed, REPLACE_BLOCKS.Num());
  return FVoxelRef(false, (int32)REPLACE_BLOCKS[index]);
}
