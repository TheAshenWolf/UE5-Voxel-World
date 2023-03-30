#pragma once
#include "UExtremeHillsOverride.h"

class UExtremeHillsWithTreesOverride : public UExtremeHillsOverride
{
public:
  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UExtremeHillsWithTreesOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::MOUNTAIN_SHRUB_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (UUtility::NextInt(chunkSeed, 10) == 0)
    {
      EStructure tree = UUtility::Structures::GetOak(chunkSeed);
      AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
    }
    else
    {
      AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(EStructure::MountainShrubOak, FIntVector(x, y, z + 1), *chunkSeed));
    }
  }
}