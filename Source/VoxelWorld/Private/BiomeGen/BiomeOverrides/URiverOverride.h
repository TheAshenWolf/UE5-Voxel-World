#pragma once
#include "UConfig.h"

class URiverOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt
  };

  static FVoxelRef GetBlockAtDepth(int32 zHeight, int32 depth, TMap<EBiome, double> allBiomes, int32 x, int32 y);
};

inline FVoxelRef URiverOverride::GetBlockAtDepth(int32 zHeight, int32 depth, TMap<EBiome, double> allBiomes, int32 x, int32 y)
{
  int32 generatedHeight = zHeight + depth;
  
  if (zHeight <= UConfig::WATER_HEIGHT + 2)
  {
    if (depth < REPLACE_BLOCKS.Num())
    {
      if (generatedHeight > UConfig::WATER_HEIGHT && AGameWorld::WorldData.Get(x, y, UConfig::WATER_HEIGHT).Color != (int32)EBasicBlock::Water)
      {
        if (allBiomes.Num() > 1)
        {
          TArray<EBiome> biomeKeys = TArray<EBiome>();
          allBiomes.GetKeys(biomeKeys);
          biomeKeys.Remove(EBiome::River);
          biomeKeys.Remove(EBiome::FrozenRiver);
          if (biomeKeys.Num() > 0)
          {
            EBiome biome = biomeKeys[0];

            return UAbsBiomeRenderer::GetBlock(zHeight, depth + zHeight, biome, allBiomes, x, y);
          }
          return FVoxelRef(false, (int32)(depth == 0 ? EBasicBlock::GrassMild : EBasicBlock::Dirt));
        }
        return FVoxelRef(false, (int32)(depth == 0 ? EBasicBlock::GrassMild : EBasicBlock::Dirt));
      }
      
      return FVoxelRef(false, (int32)REPLACE_BLOCKS[depth]);
    }
    return FVoxelRef(false, (int32)EBasicBlock::Stone);
  }
  if (allBiomes.Num() > 1 && AGameWorld::WorldData.Get(x, y, UConfig::WATER_HEIGHT).Color != (int32)EBasicBlock::Water)
  {
    TArray<EBiome> biomeKeys = TArray<EBiome>();
    allBiomes.GetKeys(biomeKeys);
    biomeKeys.Remove(EBiome::River);
    biomeKeys.Remove(EBiome::FrozenRiver);
    if (biomeKeys.Num() > 0)
    {
      EBiome biome = biomeKeys[0];

      return UAbsBiomeRenderer::GetBlock(zHeight, depth + zHeight, biome, allBiomes, x, y);
    }
    return FVoxelRef(false, (int32)(depth == 0 ? EBasicBlock::GrassMild : EBasicBlock::Dirt));
  }
  return FVoxelRef(false, (int32)(depth == 0 ? EBasicBlock::GrassMild : EBasicBlock::Dirt));
}
