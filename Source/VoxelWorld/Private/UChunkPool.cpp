#include "UChunkPool.h"

#include "UConfig.h"

void UChunkPool::SpawnChunk()
{
  AActor* actor = World->SpawnActor(AChunk::StaticClass());
  AChunk* chunk = Cast<AChunk>(actor);  
  chunk->Mesh->SetMaterial(0, Material);
  chunk->WaterMesh->SetMaterial(0, WaterMaterial);
  InactiveChunks.Add(chunk);
}

void UChunkPool::SetupPool(UWorld* world, UMaterialInterface* material, UMaterialInterface* waterMaterial)
{
  int32 chunkCount = UConfig::VERTICAL_CHUNKS * (UConfig::CHUNK_RENDER_RADIUS << 1) * (UConfig::CHUNK_RENDER_RADIUS << 1);
  
  InactiveChunks = TArray<AChunk*>();
  ActiveChunks = TArray<AChunk*>();
  World = world;
  Material = material;
  WaterMaterial = waterMaterial;
  
  for (int32 i = 0; i < chunkCount; i++)
  {
    SpawnChunk();
  }
}

AChunk* UChunkPool::RetrieveChunk()
{
  // This scenario shouldn't happen, but just in case
  if (InactiveChunks.Num() == 0)
  {
    SpawnChunk();
  }
  
  AChunk* chunk = InactiveChunks.Pop();
  ActiveChunks.Add(chunk);
  chunk->SetActorHiddenInGame(false);
  return chunk;
}

void UChunkPool::ReturnChunk(AChunk* chunk)
{
  if (chunk == nullptr)
  {
    return;
  }
  chunk->SetActorHiddenInGame(true);
  chunk->Mesh->ClearAllMeshSections();
  chunk->WaterMesh->ClearAllMeshSections();
  chunk->Vertices.Empty();
  chunk->Triangles.Empty();
  chunk->UVs.Empty();
  chunk->VertexColors.Empty();
  chunk->VerticesWater.Empty();
  chunk->TrianglesWater.Empty();
  chunk->SetActorLabel("Inactive");
  ActiveChunks.Remove(chunk);
  InactiveChunks.Add(chunk);
}

void UChunkPool::ClearPool()
{
  TArray<AChunk*> dispose = TArray<AChunk*>();
  dispose.Append(ActiveChunks);
  for (AChunk* chunk : dispose)
  {
    ReturnChunk(chunk);
  }
}
