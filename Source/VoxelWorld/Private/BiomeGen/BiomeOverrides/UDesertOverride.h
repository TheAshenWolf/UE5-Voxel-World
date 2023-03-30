#pragma once

class UDesertOverride
{
public:
   static const inline TArray<EBasicBlock> REPLACE_BLOCKS  = TArray{
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sand,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
    EBasicBlock::Sandstone,
  };

  static void GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);
};

inline void UDesertOverride::GetStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  int32 value = UUtility::NextInt(chunkSeed, UConfig::BiomeSettings::CACTUS_CHANCE);

  if (value == 0)
  {
    int32 z = UUtility::GetHeight(x, y, biome);

    
    if (AGameWorld::WorldData.Get(x, y, z).Color != (int32)EBasicBlock::Sand) return;
    z++; // Move one block above; we expect an air space
    if (!AGameWorld::WorldData.Get(x, y, z).IsEmpty) return;
   
    int32 size = 1 + UUtility::NextInt(chunkSeed, UUtility::NextInt(chunkSeed, 3) + 1);
    for (int32 i = 0; i < size; i++)
    {
      AGameWorld::WorldData.Set(x, y, z + i, FVoxelRef(false, (int32)EBasicBlock::Leaves));
    }
  }
  
}

