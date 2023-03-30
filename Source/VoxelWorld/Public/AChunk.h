#pragma once
#include "Enums/ECubeSide.h"
#include "ProceduralMeshComponent.h"
#include "VoxImport/F3DArrayRef.h"
#include "VoxImport/FVoxel.h"
#include "AChunk.generated.h"

UCLASS()
class AChunk : public AActor
{
  GENERATED_BODY()
public:
  UPROPERTY()
  FIntVector WorldPosition;
  UPROPERTY()
  TArray<FVector> Vertices;
  UPROPERTY()
  TArray<int32> Triangles;
  UPROPERTY()
  TArray<FColor> VertexColors;
  UPROPERTY()
  TArray<FVector2D> UVs;
  UPROPERTY()
  TArray<FVector> VerticesWater;
  UPROPERTY()
  TArray<int32> TrianglesWater;
  

  UPROPERTY()
  USceneComponent* Root;
  UPROPERTY()
  UProceduralMeshComponent* Mesh;
  UPROPERTY()
  UProceduralMeshComponent* WaterMesh;

  AChunk();
  AChunk* Init(FVector worldPosition);
  void DrawCube(FVoxel voxel, bool renderWater, FCriticalSection *criticalSection);
  void AddMeshData(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FColor>& vertexColors, TArray<FVector2D>& uvs, bool
                   renderWater);
  TArray<ECubeSide> GetVisibleCubeSides(FIntVector worldPosition, bool isWater);
  void Render();
};
