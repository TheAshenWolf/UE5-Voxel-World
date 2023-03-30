#pragma once
#include "EBiome.h"
#include "Enums/EBasicBlock.h"
#include "VoxImport/FVoxelRef.h"
#include "UAbsBiomeRenderer.generated.h"

UCLASS()
class UAbsBiomeRenderer : public UObject
{
  GENERATED_BODY()

public:
  UFUNCTION()
  static FVoxelRef GetBlock(int32 zHeight, int32 generatedHeight, EBiome biome, TMap<EBiome, double> allBiomes, int32 x,
                            int32 y);
  static FVoxelRef GetWaterBlock(int32 zHeight, int32 generatedHeight, EBiome biome);
  UFUNCTION()
  static FVoxelRef GetLocalBlock(const TArray<EBasicBlock> replaceBlocks, int32 depth);
  static void GenerateStructures(FIntVector chunkCoordinates);
  static void EvaluateStructure(int32 x, int32 y, int64* chunkSeed, EBiome biome);

  static inline TArray<EBasicBlock> ReplaceBlocks = TArray<EBasicBlock>{
  };
};
