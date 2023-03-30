#pragma once
#include "UConfig.h"

class UOceanOverride
{
public:
   static const inline TArray<EBasicBlock> REPLACE_BLOCKS  = TArray{
    EBasicBlock::Gravel
  };

  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UOceanOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::OCEAN_SHIPWRECK_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(EStructure::Shipwreck, FIntVector(x, y, z), *chunkSeed));
  }
}
