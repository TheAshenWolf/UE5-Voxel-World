#pragma once
#include "BiomeGen/UAbsBiomeRenderer.h"

class UMushroomIslandOverride : UAbsBiomeRenderer
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::Mycelium,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
  };
  
  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UMushroomIslandOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::MUSHROOM_ISLAND_SHROOM_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::Mycelium) return;

    EStructure shroom = UUtility::NextInt(chunkSeed, 2) == 0 ? EStructure::MushroomRed : EStructure::MushroomBrown;
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(shroom, FIntVector(x, y, z + 1), *chunkSeed));
  }
}