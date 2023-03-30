#pragma once
#include "Enums/EStructure.h"
#include "VoxImport/FVoxObject.h"

class UStructureGenerator
{
public:
  static void LoadStructures();
  static void LoadStructure(EStructure key, FString name);
  static void SpawnStructure(EStructure structure, FIntVector location, int64* chunkSeed);

protected:
  static void StructureBase(EStructure structure, FVoxObject& voxObject, FVoxObject& voxObjectD,
                            FIntVector& pivot);
  static void SpawnBasicTreeStructure(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed, FIntVector pivot, FIntVector location);
  static void SpawnBasicStructure(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed, FIntVector pivot, FIntVector location);
  static void SpawnShipwreck(FVoxObject voxObject, FVoxObject voxObjectD, int64* chunkSeed, FIntVector pivot, FIntVector location);
};
