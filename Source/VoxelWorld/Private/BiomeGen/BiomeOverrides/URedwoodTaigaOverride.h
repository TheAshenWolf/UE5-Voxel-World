#pragma once

class URedwoodTaigaOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::Podzol,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
  };

  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void URedwoodTaigaOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::REDWOOD_TAIGA_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    // if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::Podzol) return;

    if (UUtility::NextInt(chunkSeed, 10) == 0)
    {
      EStructure tree = UUtility::Structures::GetSmallSpruce(chunkSeed);
      AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
    }
    else
    {
      EStructure tree = UUtility::Structures::GetLargeSpruce(chunkSeed);
      AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
    }    
  }
}