#pragma once
#include "UPlainsOverride.h"

class UForestOverride : public UPlainsOverride
{
public:
  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UForestOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::FOREST_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::GrassMild) return;

    EStructure tree = UUtility::Structures::GetOak(chunkSeed);
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
  }
}
