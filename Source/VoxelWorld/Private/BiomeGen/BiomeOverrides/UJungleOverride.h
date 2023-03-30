#pragma once
#include "Enums/EStructure.h"

class UJungleOverride
{
public:
   static const inline TArray<EBasicBlock> REPLACE_BLOCKS  = TArray{
     EBasicBlock::GrassJungle,
     EBasicBlock::Dirt,
     EBasicBlock::Dirt,
     EBasicBlock::Dirt
  };
   static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};


inline void UJungleOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::JUNGLE_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    // if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::GrassJungle) return;
    
    int32 variant = UUtility::NextInt(chunkSeed, 18);
    
    EStructure structure;
    if (variant == 0) structure = UUtility::Structures::GetOak(chunkSeed);
    else if (variant < 4) structure = EStructure::JungleLarge;
    else if (variant < 7) structure = EStructure::JungleSmall;
    else structure = EStructure::JungleShrub;
    
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(structure, FIntVector(x, y, z + 1), *chunkSeed));
  }
}