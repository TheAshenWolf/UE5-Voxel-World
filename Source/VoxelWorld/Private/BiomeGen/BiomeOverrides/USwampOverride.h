#pragma once
#include "UConfig.h"

class USwampOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::GrassDamp,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt
  };

  static FVoxelRef GetBlockAtDepth(int32 zHeight, int32 depth);
};

inline FVoxelRef USwampOverride::GetBlockAtDepth(int32 zHeight, int32 depth)
{
  if (zHeight >= UConfig::WATER_HEIGHT - 1)
  {
    if (depth < 0) return FVoxelRef(true, 0);
    if (depth < REPLACE_BLOCKS.Num())
    {
      return FVoxelRef(false, (int32)REPLACE_BLOCKS[depth]);
    }
    return FVoxelRef(false, (int32)EBasicBlock::Stone);
  }

  if (depth < REPLACE_BLOCKS.Num())
  {
    return FVoxelRef(false, (int32)EBasicBlock::Dirt);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Stone);
}
