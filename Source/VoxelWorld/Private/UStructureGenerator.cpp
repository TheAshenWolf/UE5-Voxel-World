#include "UStructureGenerator.h"

#include "AGameWorld.h"
#include "UConfig.h"
#include "Enums/EStructure.h"
#include "Enums/EStructureDeviation.h"
#include "VoxImport/UVoxParser.h"

void UStructureGenerator::LoadStructures()
{
  if constexpr (!UConfig::GENERATE_STRUCTURES) return;

  LoadStructure(EStructure::Shipwreck, "shipwreck");
  UUtility::Structures::LoadOaks();
  UUtility::Structures::LoadSpruces();
  UUtility::Structures::LoadBirches();
  UUtility::Structures::LoadAcacias();
  UUtility::Structures::LoadJungles();
  // UUtility::Structures::LoadDarkOaks();
  UUtility::Structures::LoadMushrooms();
}

void UStructureGenerator::LoadStructure(EStructure key, FString name)
{
  FVoxObject voxObject = UVoxParser::FromFile(FPaths::ProjectContentDir() + "/Resources/Structures/" + name + ".vox");
  FVoxObject voxObjectD =
    UVoxParser::FromFile(FPaths::ProjectContentDir() + "/Resources/Structures/" + name + "_d.vox");
  AGameWorld::Structures.Add(
    key, TTuple<FVoxObject, FVoxObject, FIntVector>(voxObject, voxObjectD, FIntVector(-1, -1, -1)));
}

void UStructureGenerator::SpawnStructure(EStructure structure, FIntVector location, int64* chunkSeed)
{
  if constexpr (!UConfig::GENERATE_STRUCTURES) return;
  if (!AGameWorld::Structures.Contains(structure)) return;


  FVoxObject voxObject;
  FVoxObject voxObjectDeviation;
  FIntVector pivot;
  StructureBase(structure, voxObject, voxObjectDeviation, pivot);

  switch (structure)
  {
  case EStructure::Shipwreck:
    SpawnShipwreck(voxObject, voxObjectDeviation, chunkSeed, pivot, location);
    break;
  case EStructure::Oak0:
  case EStructure::FancyOak0:
  case EStructure::FancyOak1:
  case EStructure::OakBalloon0:
  case EStructure::Spruce0:
  case EStructure::Spruce1:
  case EStructure::Spruce2:
  case EStructure::Spruce3:
  case EStructure::Spruce4:
  case EStructure::Spruce5:
  case EStructure::SpruceLarge0:
  case EStructure::SpruceLarge1:
  case EStructure::Acacia0:
  case EStructure::Acacia1:
  case EStructure::Acacia2:
  case EStructure::Birch:
  case EStructure::MushroomRed:
  case EStructure::MushroomBrown:
    SpawnBasicTreeStructure(voxObject, voxObjectDeviation, chunkSeed, pivot, location);
    break;
  case EStructure::MountainShrubOak:
  default:
    SpawnBasicStructure(voxObject, voxObjectDeviation, chunkSeed, pivot, location);
    break;
  }
}

void UStructureGenerator::StructureBase(EStructure structure, FVoxObject& voxObject, FVoxObject& voxObjectD,
                                        FIntVector& pivot)
{
  voxObject = AGameWorld::Structures[structure].Get<0>();
  voxObjectD = AGameWorld::Structures[structure].Get<1>();
  pivot = AGameWorld::Structures[structure].Get<2>();
  if (pivot == FIntVector(-1, -1, -1))
  {
    if (!voxObjectD.Voxels.FindVoxelByValue(PivotMarker, pivot))
    {
      if (!voxObjectD.Voxels.FindVoxelByValue(ForcedPivot, pivot))
        throw std::runtime_error("Pivot not found");
    }
    else
    {
      pivot = FIntVector(pivot.X, pivot.Y, 0);
    }
  }
}

void UStructureGenerator::SpawnBasicStructure(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed,
                                              FIntVector pivot, FIntVector location)
{
  FIntVector offset = location - pivot;

  for (int32 x = 0; x < voxObject.SizeX; x++)
  {
    for (int32 y = 0; y < voxObject.SizeY; y++)
    {
      for (int32 z = 0; z < voxObject.SizeZ; z++)
      {
        FIntVector blockLocation = FIntVector(x, y, z) + offset;


        FVoxel voxel = voxObject.Voxels.Get(x, y, z);
        FVoxel deviation = voxObjectD.Voxels.Get(x, y, z);
        if (deviation.Color == EStructureDeviation::CanBeMissing)
        {
          int32 rand = UUtility::NextInt(chunkSeed, 100);
          if (rand > UConfig::BiomeSettings::BASIC_BLOCK_MISSING_CHANCE) continue;
        }
        if (voxel.IsEmpty) continue;

        bool replace = AGameWorld::WorldData.Get(blockLocation).IsEmpty || AGameWorld::WorldData.
                                                                           Get(blockLocation).Color == (int32)
          EBasicBlock::Water; // This has to be present so we replace water too
        AGameWorld::WorldData.Set(blockLocation, FVoxelRef(voxel.IsEmpty, voxel.Color), replace);
      }
    }
  }
}


void UStructureGenerator::SpawnBasicTreeStructure(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed,
                                                  FIntVector pivot, FIntVector location)
{
  FIntVector offset = location - pivot;

  int32 variableHeight = 0;
  for (int32 i = 0; i < voxObject.SizeZ; i++)
  {
    if (voxObjectD.Voxels.Get(pivot.X, pivot.Y, i).Color == (int32)EStructureDeviation::VariableHeightMarker)
    {
      variableHeight++;
    }
    else break;
  }

  int32 startingHeight = UUtility::NextInt(chunkSeed, variableHeight + 1);

  for (int32 x = 0; x < voxObject.SizeX; x++)
  {
    for (int32 y = 0; y < voxObject.SizeY; y++)
    {
      for (int32 z = startingHeight; z < voxObject.SizeZ; z++)
      {
        FIntVector blockLocation = FIntVector(x, y, z - startingHeight) + offset;


        FVoxel voxel = voxObject.Voxels.Get(x, y, z);
        FVoxel deviation = voxObjectD.Voxels.Get(x, y, z);
        if (deviation.Color == EStructureDeviation::CanBeMissing)
        {
          int32 rand = UUtility::NextInt(chunkSeed, 100);
          if (rand > UConfig::BiomeSettings::BASIC_BLOCK_MISSING_CHANCE) continue;
        }
        if (voxel.IsEmpty) continue;

        bool replace = AGameWorld::WorldData.Get(blockLocation).IsEmpty || AGameWorld::WorldData.
                                                                           Get(blockLocation).Color == (int32)
          EBasicBlock::Water; // This has to be present so we replace water too
        AGameWorld::WorldData.Set(blockLocation, FVoxelRef(voxel.IsEmpty, voxel.Color), replace);
      }
    }
  }
}

void UStructureGenerator::SpawnShipwreck(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed,
                                         FIntVector pivot, FIntVector location)
{
  bool point1 = UUtility::NextInt(chunkSeed, 100) < UConfig::BiomeSettings::SHIPWRECK_MAST_CHANCE;
  bool point2 = UUtility::NextInt(chunkSeed, 100) < UConfig::BiomeSettings::SHIPWRECK_MAST_CHANCE;

  FIntVector offset = location - pivot;

  for (int32 x = 0; x < voxObject.SizeX; x++)
  {
    for (int32 y = 0; y < voxObject.SizeY; y++)
    {
      for (int32 z = 0; z < voxObject.SizeZ; z++)
      {
        FIntVector blockLocation = FIntVector(x, y, z) + offset;


        FVoxel voxel = voxObject.Voxels.Get(x, y, z);
        FVoxel deviation = voxObjectD.Voxels.Get(x, y, z);
        if (deviation.Color == SpecialCase && !point1) continue;
        if (deviation.Color == SpecialCase2 && !point2) continue;
        if (deviation.Color == EStructureDeviation::CanBeMissing)
        {
          int32 rand = UUtility::NextInt(chunkSeed, 100);
          if (rand > UConfig::BiomeSettings::BASIC_BLOCK_MISSING_CHANCE) continue;
        }
        if (voxel.IsEmpty) continue;

        bool replace = AGameWorld::WorldData.Get(blockLocation).IsEmpty || AGameWorld::WorldData.
                                                                           Get(blockLocation).Color == (int32)
          EBasicBlock::Water; // This has to be present so we replace water too
        AGameWorld::WorldData.Set(blockLocation, FVoxelRef(voxel.IsEmpty, voxel.Color), replace);
      }
    }
  }
}
