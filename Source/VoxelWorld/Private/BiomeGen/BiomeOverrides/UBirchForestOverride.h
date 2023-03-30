#pragma once
#include "UForestOverride.h"

class UBirchForestOverride : public UForestOverride
{  
public:
  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UBirchForestOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::BIRCH_FOREST_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::GrassMild) return;

    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(EStructure::Birch, FIntVector(x, y, z + 1), *chunkSeed));
  }
}
