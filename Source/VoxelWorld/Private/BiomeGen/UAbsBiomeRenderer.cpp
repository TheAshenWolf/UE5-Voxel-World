#include "BiomeGen/UAbsBiomeRenderer.h"

#include "UStructureGenerator.h"
#include "Enums/EBasicBlock.h"

#include "BiomeOverrides/UBeachOverride.h"
#include "BiomeOverrides/UBirchForestHillsOverride.h"
#include "BiomeOverrides/UBirchForestOverride.h"
#include "BiomeOverrides/UColdBeachOverride.h"
#include "BiomeOverrides/UDeepOceanOverride.h"
#include "BiomeOverrides/UDesertHillsOverride.h"
#include "BiomeOverrides/UDesertOverride.h"
#include "BiomeOverrides/UExtremeHillsOverride.h"
#include "BiomeOverrides/UExtremeHillsWithTreesOverride.h"
#include "BiomeOverrides/UForestHillsOverride.h"
#include "BiomeOverrides/UForestOverride.h"
#include "BiomeOverrides/UFrozenOceanOverride.h"
#include "BiomeOverrides/UFrozenRiverOverride.h"
#include "BiomeOverrides/UIceMountainsOverride.h"
#include "BiomeOverrides/UIcePlainsOverride.h"
#include "BiomeOverrides/UJungleEdgeOverride.h"
#include "BiomeOverrides/UJungleHillsOverride.h"
#include "BiomeOverrides/UJungleOverride.h"
#include "BiomeOverrides/UMesaClearRockOverride.h"
#include "BiomeOverrides/UMesaOverride.h"
#include "BiomeOverrides/UMesaRockOverride.h"
#include "BiomeOverrides/UMushroomIslandOverride.h"
#include "BiomeOverrides/UMushroomShoreOverride.h"
#include "BiomeOverrides/UOceanOverride.h"
#include "BiomeOverrides/UPlainsOverride.h"
#include "BiomeOverrides/URedwoodTaigaHillsOverride.h"
#include "BiomeOverrides/URedwoodTaigaOverride.h"
#include "BiomeOverrides/URiverOverride.h"
#include "BiomeOverrides/URoofedForestOverride.h"
#include "BiomeOverrides/USavannaOverride.h"
#include "BiomeOverrides/USavannaRockOverride.h"
#include "BiomeOverrides/USmallerExtremeHillsOverride.h"
#include "BiomeOverrides/UStoneBeachOverride.h"
#include "BiomeOverrides/USwampOverride.h"
#include "BiomeOverrides/UTaigaColdHillsOverride.h"
#include "BiomeOverrides/UTaigaColdOverride.h"
#include "BiomeOverrides/UTaigaHillsOverride.h"
#include "BiomeOverrides/UTaigaOverride.h"

FVoxelRef UAbsBiomeRenderer::GetBlock(int32 zHeight, int32 generatedHeight, EBiome biome,
                                      TMap<EBiome, double> allBiomes, int32 x, int32 y)
{
  if (zHeight == 0)
  {
    return FVoxelRef(false, UConfig::BIOME_COLORS[(int32)biome]); // A biome map at the bottom of the world;
  }
  
  int32 depth = generatedHeight - zHeight;
  switch (biome)
  {
  case EBiome::Ocean: return GetLocalBlock(UOceanOverride::REPLACE_BLOCKS, depth);
  case EBiome::Plains: return GetLocalBlock(UPlainsOverride::REPLACE_BLOCKS, depth);
  case EBiome::Desert: return GetLocalBlock(UDesertOverride::REPLACE_BLOCKS, depth);
  case EBiome::ExtremeHills: return GetLocalBlock(UExtremeHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::Forest: return GetLocalBlock(UForestOverride::REPLACE_BLOCKS, depth);
  case EBiome::Taiga: return GetLocalBlock(UTaigaOverride::REPLACE_BLOCKS, depth);
  case EBiome::Swampland: return USwampOverride::GetBlockAtDepth(zHeight, depth);
  case EBiome::River: return URiverOverride::GetBlockAtDepth(zHeight, depth,allBiomes, x, y);
  case EBiome::Hell: return GetLocalBlock(ReplaceBlocks, depth); // unused
  case EBiome::Sky: return GetLocalBlock(ReplaceBlocks, depth); // unused
  case EBiome::FrozenOcean: return GetLocalBlock(UFrozenOceanOverride::REPLACE_BLOCKS, depth);
  case EBiome::FrozenRiver: return UFrozenRiverOverride::GetBlockAtDepth(zHeight, depth,allBiomes, x, y);
  case EBiome::IcePlains: return GetLocalBlock(UIcePlainsOverride::REPLACE_BLOCKS, depth);
  case EBiome::IceMountains: return GetLocalBlock(UIceMountainsOverride::REPLACE_BLOCKS, depth);
  case EBiome::MushroomIsland: return GetLocalBlock(UMushroomIslandOverride::REPLACE_BLOCKS, depth);
  case EBiome::MushroomIslandShore: return UMushroomShoreOverride::GetBlockAtDepth(zHeight, depth);
  case EBiome::Beach: return GetLocalBlock(UBeachOverride::REPLACE_BLOCKS, depth);
  case EBiome::DesertHills: return GetLocalBlock(UDesertHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::ForestHills: return GetLocalBlock(UForestHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::TaigaHills: return GetLocalBlock(UTaigaHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::SmallerExtremeHills: return GetLocalBlock(USmallerExtremeHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::Jungle: return GetLocalBlock(UJungleOverride::REPLACE_BLOCKS, depth);
  case EBiome::JungleHills: return GetLocalBlock(UJungleHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::JungleEdge: return GetLocalBlock(UJungleEdgeOverride::REPLACE_BLOCKS, depth);
  case EBiome::DeepOcean:  return GetLocalBlock(UDeepOceanOverride::REPLACE_BLOCKS, depth);
  case EBiome::StoneBeach: return GetLocalBlock(UStoneBeachOverride::REPLACE_BLOCKS, depth);
  case EBiome::ColdBeach: return GetLocalBlock(UBeachOverride::REPLACE_BLOCKS, depth);
  case EBiome::BirchForest: return GetLocalBlock(UBirchForestOverride::REPLACE_BLOCKS, depth);
  case EBiome::BirchForestHills: return GetLocalBlock(UBirchForestHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::RoofedForest: return GetLocalBlock(URoofedForestOverride::REPLACE_BLOCKS, depth);
  case EBiome::TaigaCold: return GetLocalBlock(UTaigaColdOverride::REPLACE_BLOCKS, depth);
  case EBiome::TaigaColdHills: return GetLocalBlock(UTaigaColdHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::RedwoodTaiga: return GetLocalBlock(URedwoodTaigaOverride::REPLACE_BLOCKS, depth);
  case EBiome::RedwoodTaigaHills: return GetLocalBlock(URedwoodTaigaHillsOverride::REPLACE_BLOCKS, depth);
  case EBiome::ExtremeHillsWithTrees: return GetLocalBlock(UExtremeHillsWithTreesOverride::REPLACE_BLOCKS, depth);
  case EBiome::Savanna: return GetLocalBlock(USavannaOverride::REPLACE_BLOCKS, depth);
  case EBiome::SavannaRock: return GetLocalBlock(USavannaRockOverride::REPLACE_BLOCKS, depth);
  case EBiome::Mesa: return UMesaOverride::GetBlockAtDepth(zHeight, depth);
  case EBiome::MesaRock: return UMesaRockOverride::GetBlockAtDepth(zHeight, depth);
  case EBiome::MesaClearRock: UMesaClearRockOverride::GetBlockAtDepth(zHeight, depth);
  default: return FVoxelRef(false, (int32)EBasicBlock::Netherrack);
  }
}

FVoxelRef UAbsBiomeRenderer::GetWaterBlock(int32 zHeight, int32 generatedHeight, EBiome biome)
{
  switch (biome)
  {
  case EBiome::FrozenOcean: return UFrozenOceanOverride::GetWaterBlocks(zHeight);
  case EBiome::FrozenRiver: return UFrozenRiverOverride::GetWaterBlocks(zHeight);
  case EBiome::ColdBeach: return UColdBeachOverride::GetWaterBlocks(zHeight);
  default: return FVoxelRef(false, (int32)EBasicBlock::Water);
  }
}

FVoxelRef UAbsBiomeRenderer::GetLocalBlock(const TArray<EBasicBlock> replaceBlocks, int32 depth)
{

  if (depth < 0) return FVoxelRef(true, 0);
  if (depth < replaceBlocks.Num())
  {
    return FVoxelRef(false, (int32)replaceBlocks[depth]);
  }
  return FVoxelRef(false, (int32)EBasicBlock::Stone);
}

void UAbsBiomeRenderer::GenerateStructures(FIntVector chunkCoordinates)
{
  chunkCoordinates *= UConfig::CHUNK_SIZE;
  
  for (int32 x = chunkCoordinates.X; x < chunkCoordinates.X + UConfig::CHUNK_SIZE; x++)
  {
    for (int32 y = chunkCoordinates.Y; y < chunkCoordinates.Y + UConfig::CHUNK_SIZE; y++)
    {
      int64 chunkSeed = UUtility::GetChunkSeed(x, y);
      if (!AGameWorld::BiomeMask.Contains(FIntVector2(x, y))) continue; // this happens if player moves too quickly... cant happen under normal circumstances
      EBiome biome = AGameWorld::BiomeMask[FIntVector2(x, y)];
      EvaluateStructure(x, y, &chunkSeed, biome);
    }
  }
}

void UAbsBiomeRenderer::EvaluateStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome)
{
  switch (biome)
  {
  case EBiome::Ocean: UOceanOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::Plains: UPlainsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::Desert: UDesertOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::ExtremeHills: break; // No structures
  case EBiome::Forest: UForestOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::Taiga: UTaigaOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  // case EBiome::Swampland: break;
  case EBiome::River: break; // No structures
  case EBiome::Hell: break; // No structures
  case EBiome::Sky: break; // No structures
  case EBiome::FrozenOcean: UFrozenOceanOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::FrozenRiver: break; // No structures
  case EBiome::IcePlains: break; // No structures
  case EBiome::IceMountains: break; // No structures
  case EBiome::MushroomIsland: UMushroomIslandOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::MushroomIslandShore: break; // No structures
  case EBiome::Beach: break; // No structures
  case EBiome::DesertHills: UDesertHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::ForestHills: UForestHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::TaigaHills: UTaigaHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::SmallerExtremeHills: break; // No structures
  case EBiome::Jungle: UJungleOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::JungleHills: UJungleHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::JungleEdge: UJungleEdgeOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::DeepOcean: UDeepOceanOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::StoneBeach: break; // No structures
  case EBiome::ColdBeach: break; // No structures
  case EBiome::BirchForest: UBirchForestOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::BirchForestHills: UBirchForestHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  // case EBiome::RoofedForest: break;
  case EBiome::TaigaCold: UTaigaColdOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::TaigaColdHills: break;
  case EBiome::RedwoodTaiga: URedwoodTaigaOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::RedwoodTaigaHills: URedwoodTaigaHillsOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::ExtremeHillsWithTrees: UExtremeHillsWithTreesOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::Savanna: USavannaOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::SavannaRock: USavannaRockOverride::GetStructure(x, y, chunkSeed, biome);
    break;
  case EBiome::Mesa: break; // No structures
  case EBiome::MesaRock: break; // No structures
  case EBiome::MesaClearRock: break; // No structures
  default: ;
  }

  while (!AGameWorld::StructuresToSpawn.IsEmpty())
  {
    TTuple<EStructure, FIntVector, int64> tuple;
    AGameWorld::StructuresToSpawn.Dequeue(tuple);

    UStructureGenerator::SpawnStructure(tuple.Get<0>(), tuple.Get<1>(), &tuple.Get<2>());
  }
}
