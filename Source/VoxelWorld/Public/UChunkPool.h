#pragma once
#include "AChunk.h"
#include "UChunkPool.generated.h"

UCLASS()
class UChunkPool : public UObject
{
  GENERATED_BODY()
private:
  UPROPERTY()
  UWorld* World = nullptr;
  UPROPERTY()
  UMaterialInterface* Material = nullptr;
  UPROPERTY()
  UMaterialInterface* WaterMaterial = nullptr;
  void SpawnChunk();
  
public:
  UPROPERTY()
  TArray<AChunk*> InactiveChunks;
  UPROPERTY()
  TArray<AChunk*> ActiveChunks;

  void SetupPool(UWorld* world, UMaterialInterface* material, UMaterialInterface* waterMaterial);
  AChunk* RetrieveChunk();
  void ReturnChunk(AChunk* chunk);
  void ClearPool();
};