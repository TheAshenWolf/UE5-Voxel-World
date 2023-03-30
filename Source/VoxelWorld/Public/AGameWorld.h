#pragma once

#include "CoreMinimal.h"
#include "AChunk.h"
#include "Enums/EChunkState.h"
#include "Enums/EStructure.h"
#include "UChunkPool.h"
#include "UUtility.h"
#include "VoxelCharacter.h"
#include "BiomeGen/UAbsBiomeRenderer.h"
#include "BiomeGen/EBiome.h"
#include "GameFramework/Actor.h"
#include "VoxelWorld/Public/VoxImport/FVoxObject.h"
#include "VoxImport/F3DArrayRef.h"
#include "AGameWorld.generated.h"


UCLASS()
class VOXELWORLD_API AGameWorld : public AActor
{
  GENERATED_BODY()

public:
  AGameWorld()
  {
    PrimaryActorTick.bCanEverTick = true;
  }

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  UMaterialInterface* Material;
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  UMaterialInterface* WaterMaterial;
  UPROPERTY()
  UChunkPool* ChunkPool;
  UPROPERTY()
  int64 Seed;
  inline static F3DArrayRef WorldData = F3DArrayRef();
  UPROPERTY()
  F3DArrayRef Changes;
  UPROPERTY()
  bool PendingChanges;
  UPROPERTY()
  FIntVector LastRenderCallPosition;
  TMap<FIntVector, TTuple<EChunkState, AChunk*>> ChunkStates;
  static inline TMap<FIntVector2, EBiome> BiomeMask = TMap<FIntVector2, EBiome>();
  // Type of structure : (StructureObject, StructureObjectDeviation)
  static inline TMap<EStructure, TTuple<FVoxObject, FVoxObject, FIntVector>> Structures = TMap<EStructure, TTuple<FVoxObject, FVoxObject, FIntVector>>();
  UPROPERTY()
  UUtility* Utility;
  UPROPERTY()
  AVoxelCharacter* PlayerCharacter;
  UPROPERTY()
  double TimeSinceLastRender = 0;
  UPROPERTY()
  FVector LastPlayerPosition;
  TQueue<TTuple<AChunk*, FIntVector>> ChunksToRender;
  TQueue<FIntVector> ChunksToGenerateData;
  TQueue<FIntVector> ChunksToGenerateBiomes;
  UPROPERTY()
  bool LoadingWorld = false;
  UPROPERTY()
  UAbsBiomeRenderer* BiomeRenderer;

  UPROPERTY()
  FString FilePath = "C:/tmp/game.json";

  inline static TQueue<TTuple<EStructure, FIntVector, int64>> StructuresToSpawn = TQueue<TTuple<EStructure, FIntVector, int64>>();
  inline static TMap<FIntVector2, int32> WorldHeightMap = TMap<FIntVector2, int32>();


  UFUNCTION() void SaveGame();
  UFUNCTION() void LoadGameFromDefaultFile();
  void LoadGameFromJson(FString json);
  UFUNCTION() void RenderChunks(FIntVector center, bool initialRender = false);
  UFUNCTION() void LoadChunks(FIntVector center, bool initialLoad = false);
  UFUNCTION() void LoadBiomes(FIntVector center, bool initialLoad = false);
  UFUNCTION() TMap<FIntVector2, EBiome> GenerateBiomeMask(FIntVector center);
  UFUNCTION() void GenerateChunkData(FIntVector chunkCoordinates);
  UFUNCTION() F3DArrayRef GenerateColumn(int32 x, int32 y, FIntVector chunkCoordinates, int32 height, EBiome prevailingBiome, const TMap<EBiome, double>& biomes);
  UFUNCTION() void CleanupChunks();
  UFUNCTION() void RefreshChunkBlob(FIntVector chunkCoordinates);
  UFUNCTION() void RemoveVoxel(FVector location);
  UFUNCTION() void TickGeneration();

protected:
  // Called when the game starts or when spawned
  UFUNCTION() virtual void BeginPlay() override;
  UFUNCTION() virtual void Tick(float deltaSeconds) override;
};
