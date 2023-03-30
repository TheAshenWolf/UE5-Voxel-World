#pragma once
#include "UConfig.h"

class UMushroomShoreOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::Mycelium,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
  };

  static FVoxelRef GetBlockAtDepth(int32 zHeight, int32 depth);
};

inline FVoxelRef UMushroomShoreOverride::GetBlockAtDepth(int32 zHeight, int32 depth)
{
  if (zHeight >= UConfig::WATER_HEIGHT - 1)
  {
    if (depth < REPLACE_BLOCKS.Num())
    {
      return FVoxelRef(false, (int32)REPLACE_BLOCKS[depth]);
    }
    return FVoxelRef(false, (int32)EBasicBlock::Stone);
  }

  if (depth < REPLACE_BLOCKS.Num())
  {
    return FVoxelRef(false, (int32)EBasicBlock::Sand);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Stone);
}
