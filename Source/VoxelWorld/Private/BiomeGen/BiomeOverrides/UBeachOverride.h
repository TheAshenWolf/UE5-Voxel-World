#pragma once
#include "BiomeGen/UAbsBiomeRenderer.h"

class UBeachOverride
{
public:
   static const inline TArray<EBasicBlock> REPLACE_BLOCKS  = TArray{
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sandstone,
  };
};
