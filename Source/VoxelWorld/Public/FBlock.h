#pragma once
#include "AChunk.h"
#include "Enums/ECubeSide.h"
#include "UConfig.h"
#include "VoxImport/FVoxel.h"
#include "VoxImport/UVoxFormatUtilities.h"
#include "FBlock.generated.h"

USTRUCT()
struct FBlock
{
  GENERATED_BODY()

public:
  UPROPERTY()
  FVoxel VoxelData;
  UPROPERTY()
  AChunk* Owner;
  UPROPERTY()
  FVector PositionWithinChunk;

  FBlock(FVector positionWithinChunk, AChunk* owner, FVoxel voxel);
  void CreateQuad(ECubeSide side, FVoxel voxel, TArray<FVector>& vertices, TArray<int32>& triangles,
                  TArray<FColor>& vertexColors, TArray<FVector2D>& uvs, bool containsTopFaceAndIsWater);
  FBlock();
};
