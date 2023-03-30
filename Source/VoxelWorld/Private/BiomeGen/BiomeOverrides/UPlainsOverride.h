#pragma once
#include "AGameWorld.h"
#include "UConfig.h"

class UPlainsOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::GrassMild,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt
  };

  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UPlainsOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::PLAINS_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::GrassMild) return;

    EStructure tree = UUtility::NextInt(chunkSeed, 10) == 0 ? EStructure::Birch : UUtility::Structures::GetOak(chunkSeed);
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
  }
}
