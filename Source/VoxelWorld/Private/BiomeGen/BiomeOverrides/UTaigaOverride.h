#pragma once

class UTaigaOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::GrassCold,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt,
  };
  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UTaigaOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::TAIGA_TREE_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    if (!AGameWorld::WorldData.Get(x, y, z + 1).IsEmpty) return;
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::GrassCold) return;

    EStructure tree = UUtility::Structures::GetSmallSpruce(chunkSeed);
    AGameWorld::StructuresToSpawn.Enqueue(TTuple<EStructure, FIntVector, int64>(tree, FIntVector(x, y, z + 1), *chunkSeed));
  }
}
