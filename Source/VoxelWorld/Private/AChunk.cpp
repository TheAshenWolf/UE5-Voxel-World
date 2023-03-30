#include "AChunk.h"

#include "FBlock.h"
#include "AGameWorld.h"
#include "UConfig.h"

AChunk::AChunk()
{
  Root = CreateDefaultSubobject<USceneComponent>("SceneComponent");
  RootComponent = Root;
  Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Mesh");
  WaterMesh = CreateDefaultSubobject<UProceduralMeshComponent>("WaterMesh");

  Mesh->SetupAttachment(Root);
  WaterMesh->SetupAttachment(Root);

  AddInstanceComponent(Mesh);
  AddInstanceComponent(WaterMesh);

  WorldPosition = FIntVector(0, 0, 0);
}

AChunk* AChunk::Init(FVector worldPosition)
{
  this->WorldPosition = FIntVector(worldPosition);
  SetActorLocation(FVector(WorldPosition * 100));
  return this;
}


void AChunk::AddMeshData(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FColor>& vertexColors,
                         TArray<FVector2D>& uvs,
                         bool renderWater)
{
  int32 lastVertexIndex;
  if (renderWater)
  {
    lastVertexIndex = VerticesWater.Num();
    VerticesWater.Append(vertices);
  }
  else
  {
    lastVertexIndex = Vertices.Num();
    Vertices.Append(vertices);
    VertexColors.Append(vertexColors);
  }


  if (vertices.Num() == 4)
  {
    for (const int32 triangle : triangles)
    {
      if (renderWater) TrianglesWater.Add(triangle + lastVertexIndex);
      else Triangles.Add(triangle + lastVertexIndex);
    }
    UVs.Append(uvs);
  }

  vertices.Empty();
  triangles.Empty();
  vertexColors.Empty();
  uvs.Empty();
}

TArray<ECubeSide> AChunk::GetVisibleCubeSides(FIntVector worldPosition, bool isWater)
{
  if (AGameWorld::WorldData.Get(worldPosition).IsEmpty) return TArray<ECubeSide>();
  TArray<ECubeSide> renderableSides = TArray<ECubeSide>();

  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(0, 0, 1)).IsEmptyOrWater(isWater) || (AGameWorld::WorldData.Get(worldPosition).
    Color == (int32)EBasicBlock::Water && AGameWorld::WorldData.Get(worldPosition + FIntVector(0, 0, 1)).Color != (int32)EBasicBlock::Water))
  {
    renderableSides.Add(Up);
  }
  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(0, 0, -1)).IsEmptyOrWater(isWater))
  {
    renderableSides.Add(Down);
  }


  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(1, 0, 0)).IsEmptyOrWater(isWater))
  {
    renderableSides.Add(Front);
  }
  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(-1, 0, 0)).IsEmptyOrWater(isWater))
  {
    renderableSides.Add(Back);
  }


  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(0, 1, 0)).IsEmptyOrWater(isWater))
  {
    renderableSides.Add(Right);
  }
  if (AGameWorld::WorldData.Get(worldPosition + FIntVector(0, -1, 0)).IsEmptyOrWater(isWater))
  {
    renderableSides.Add(Left);
  }


  return renderableSides;
}

void AChunk::Render()
{
  ensure(Mesh != nullptr);
  ensure(WaterMesh != nullptr);

  Vertices = TArray<FVector>();
  Triangles = TArray<int32>();
  VertexColors = TArray<FColor>();

  VerticesWater = TArray<FVector>();
  TrianglesWater = TArray<int32>();
  UVs = TArray<FVector2D>();


  FCriticalSection criticalSection;
  ParallelFor(UConfig::CHUNK_VOLUME, [&](int32 index)
  {
    int32 x = index % UConfig::CHUNK_SIZE;
    int32 y = (index / UConfig::CHUNK_SIZE) % UConfig::CHUNK_SIZE;
    int32 z = index / (UConfig::CHUNK_SIZE * UConfig::CHUNK_SIZE);


    FIntVector position = FIntVector(x, y, z) + WorldPosition;
    FVoxel voxel = FVoxel(position, AGameWorld::WorldData.Get(position));
    DrawCube(voxel, voxel.Color == (int32)EBasicBlock::Water, &criticalSection);
  });

  Mesh->CreateMeshSection(0, Vertices, Triangles,
                          TArray<FVector>(), TArray<FVector2D>(), VertexColors,
                          TArray<FProcMeshTangent>(),
                          true
  );
  WaterMesh->CreateMeshSection(0, VerticesWater, TrianglesWater,
                               TArray<FVector>(), UVs, VertexColors,
                               TArray<FProcMeshTangent>(),
                               false
  );


  Mesh->SetMeshSectionVisible(0, true);
  WaterMesh->SetMeshSectionVisible(0, true);
}

void AChunk::DrawCube(FVoxel voxel, bool renderWater, FCriticalSection* criticalSection)
{
  if (voxel.IsEmpty) return; // No need to check for anything, if the voxel is empty
  if (voxel.Color == (int32)EBasicBlock::Water && !renderWater) return;
  if (renderWater && voxel.Color != (int32)EBasicBlock::Water) return;

  TArray<ECubeSide> renderableSides = GetVisibleCubeSides(voxel.GetIntPosition(), renderWater);
  if (renderableSides.Num() == 0) return; // No need to create variables when we are not going to use them
  FBlock block = FBlock(voxel.GetPosition(), this, voxel);
  TArray<FVector> vertices;
  TArray<int32> triangles;
  TArray<FColor> vertexColors;
  TArray<FVector2D> uvs;
  for (ECubeSide side : renderableSides)
  {
    block.CreateQuad(side, voxel, vertices, triangles, vertexColors, uvs,
                     renderableSides.Contains(ECubeSide::Up) && renderWater);
    criticalSection->Lock();
    AddMeshData(vertices, triangles, vertexColors, uvs, renderWater);
    criticalSection->Unlock();
  }
}
