#include "FBlock.h"


FBlock::FBlock(FVector positionWithinChunk, AChunk* owner, FVoxel voxel)
{
  Owner = owner;
  PositionWithinChunk = positionWithinChunk;
  VoxelData = voxel;
}

FBlock::FBlock(): Owner(nullptr), PositionWithinChunk()
{
}

void FBlock::CreateQuad(ECubeSide side, FVoxel voxel, TArray<FVector>& vertices, TArray<int32>& triangles,
                        TArray<FColor>& vertexColors, TArray<FVector2D>& uvs, bool containsTopFaceAndIsWater)
{
  if (voxel.IsEmpty) return;

  FIntVector intPosition = voxel.GetIntPosition();

  FVector position = FVector(intPosition.X % UConfig::CHUNK_SIZE, intPosition.Y % UConfig::CHUNK_SIZE,
                             intPosition.Z % UConfig::CHUNK_SIZE);
  if (position.X < 0) position.X += UConfig::CHUNK_SIZE;
  if (position.Y < 0) position.Y += UConfig::CHUNK_SIZE;

  FVector v0 = FVector(0, 1, 0);
  FVector v1 = FVector(0, 0, 0);
  FVector v2 = FVector(1, 1, 0);
  FVector v3 = FVector(1, 0, 0);
  FVector v4 = FVector(0, 1, 1);
  FVector v5 = FVector(0, 0, 1);
  FVector v6 = FVector(1, 1, 1);
  FVector v7 = FVector(1, 0, 1);

  if (containsTopFaceAndIsWater) // This lowers the top four vertices of the block by 1/8 of the block height
  {
    v4 *= FVector(1, 1, 0.875);
    v5 *= FVector(1, 1, 0.875);
    v6 *= FVector(1, 1, 0.875);
    v7 *= FVector(1, 1, 0.875);
  }

  v0 = (v0 + position) * 100;
  v1 = (v1 + position) * 100;
  v2 = (v2 + position) * 100;
  v3 = (v3 + position) * 100;
  v4 = (v4 + position) * 100;
  v5 = (v5 + position) * 100;
  v6 = (v6 + position) * 100;
  v7 = (v7 + position) * 100;

  FColor color = UVoxFormatUtilities::PALETTE[voxel.Color];
  switch (side)
  {
  case Up:
    vertices = {v5, v4, v6, v7};
    break;
  case Down:
    vertices = {v3, v2, v0, v1};
    break;
  case Left:
    vertices = {v5, v7, v3, v1};
    break;
  case Right:
    vertices = {v6, v4, v0, v2};
    break;
  case Front:
    vertices = {v7, v6, v2, v3};
    break;
  case Back:
    vertices = {v4, v5, v1, v0};
    break;
  default:
    throw std::exception("The Dark Side is not an option. Sadly.");
  }

  if (containsTopFaceAndIsWater)
  uvs = {
    FVector2D(1, 1),
    FVector2D(0, 1),
    FVector2D(0, 0),
    FVector2D(1, 0),
  };
  vertexColors = {color, color, color, color};
  triangles = {0, 1, 3, 1, 2, 3};
}
