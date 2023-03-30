// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelWorld/Public/AGameWorld.h"

#include "Enums/EChunkState.h"
#include "FGameSave.h"
#include "JsonObjectConverter.h"
#include "UConfig.h"
#include "UStructureGenerator.h"
#include "UUtility.h"
#include "BiomeGen/UAbsBiomeRenderer.h"
#include "BiomeGen/UBiomeGenerator.h"
#include "Enums/EBasicBlock.h"
#include "Enums/EStructure.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

using namespace UE::Tasks;

void AGameWorld::BeginPlay()
{
  Super::BeginPlay();

  LoadingWorld = true;

  PlayerCharacter = Cast<AVoxelCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
  PlayerCharacter->OnDig.AddUniqueDynamic(this, &AGameWorld::RemoveVoxel);

  PlayerCharacter->OnGameSave.AddUniqueDynamic(this, &AGameWorld::SaveGame);
  PlayerCharacter->OnGameLoad.AddUniqueDynamic(this, &AGameWorld::LoadGameFromDefaultFile);

  BiomeRenderer = NewObject<UAbsBiomeRenderer>();


  Utility = NewObject<UUtility>();
  UUtility::SetWorldSeed(UConfig::WORLD_SEED_OVERRIDE);
  ChunkPool = NewObject<UChunkPool>();
  ChunkPool->SetupPool(GetWorld(), Material, WaterMaterial);
  Seed = UUtility::GetSeed();

  // For some reason, this has to be here
  BiomeMask = TMap<FIntVector2, EBiome>();
  WorldData = F3DArrayRef();
  ChunkStates = TMap<FIntVector, TTuple<EChunkState, AChunk*>>();
  Changes.Empty();
  ChunksToRender.Empty();
  ChunksToGenerateBiomes.Empty();
  ChunksToGenerateData.Empty();

  UStructureGenerator::LoadStructures(); // Loads all structures from their respective files into memory

  int64 seed = UUtility::GetSeed();

  int32 x = UUtility::NextInt(&seed, UConfig::SPAWN_DEVIATION) - (UConfig::SPAWN_DEVIATION >> 1);
  int32 y = UUtility::NextInt(&seed, UConfig::SPAWN_DEVIATION) - (UConfig::SPAWN_DEVIATION >> 1);

  PlayerCharacter->SetActorLocation(
    FVector(x * 100, y * 100, Utility->GetHeightAtPoint(FVector2D(x, y), EBiome::ExtremeHills) * 100 + 1000));
  FIntVector playerPos = FIntVector(PlayerCharacter->GetActorLocation() / 100);

  LoadBiomes(playerPos, true);
  LoadChunks(playerPos, true);
  RenderChunks(playerPos, true); // Renders all chunks
  LoadingWorld = false;
}

void AGameWorld::Tick(float deltaSeconds)
{
  DrawDebugSphere(GetWorld(), PlayerCharacter->GetActorLocation(),
                  UConfig::CHUNK_RENDER_RADIUS * UConfig::CHUNK_SIZE * 100,
                  32, FColor::Red, false, 1);

  Super::Tick(deltaSeconds);
  if (LoadingWorld)
  {
    return;
  }

  TickGeneration();

  if (TimeSinceLastRender >= UConfig::CHUNK_REFRESH_RATE) // We dont need to recalculate all the chunks every frame.
  {
    if (LastPlayerPosition == PlayerCharacter->GetActorLocation())
    // If the player hasn't moved since the last render, we dont need to render again.
    {
      TimeSinceLastRender = 0;
      return;
    }
    LastPlayerPosition = PlayerCharacter->GetActorLocation();
    FIntVector playerPosition = FIntVector(PlayerCharacter->GetActorLocation() * 0.01f);

    CleanupChunks();
    LoadBiomes(playerPosition);
    LoadChunks(playerPosition);
    RenderChunks(playerPosition); // Renders all chunks

    TimeSinceLastRender = 0;
  }
  else
  {
    TimeSinceLastRender += deltaSeconds;
  }
}


void AGameWorld::SaveGame()
{
  FGameSave save = FGameSave();
  save.Seed = Seed;
  save.Changes = Changes;
  save.PlayerPosition = PlayerCharacter->GetActorLocation();

  TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(save);
  FString jsonString;
  FJsonSerializer::Serialize(jsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&jsonString, 0));
  FFileHelper::SaveStringToFile(jsonString, *FilePath);
}


void AGameWorld::LoadGameFromDefaultFile()
{
  FString Result;
  FFileHelper::LoadFileToString(Result, *FilePath);

  LoadGameFromJson(Result);
}

void AGameWorld::LoadGameFromJson(FString json)
{
  TSharedPtr<FJsonObject> RetJsonObject;
  FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(json), RetJsonObject);

  LoadingWorld = true;
  PlayerCharacter->SetActorEnableCollision(false);

  FGameSave save;
  FJsonObjectConverter::JsonObjectToUStruct<FGameSave>(RetJsonObject.ToSharedRef(), &save);

  ChunkPool->ClearPool();

  FVector pos = save.PlayerPosition + FVector(0, 0, 1000);

  // Utility = NewObject<UUtility>();
  UUtility::SetWorldSeed(save.Seed);
  Seed = UUtility::GetSeed();

  BiomeMask = TMap<FIntVector2, EBiome>();
  WorldData = F3DArrayRef();
  ChunkStates = TMap<FIntVector, TTuple<EChunkState, AChunk*>>();
  Changes = save.Changes;
  ChunksToRender.Empty();
  ChunksToGenerateBiomes.Empty();
  ChunksToGenerateData.Empty();

  PlayerCharacter->TeleportTo(pos, PlayerCharacter->GetActorRotation(), false, true);

  FIntVector playerPos = FIntVector(PlayerCharacter->GetActorLocation() / 100);

  LoadBiomes(playerPos, true);
  LoadChunks(playerPos, true);
  RenderChunks(playerPos, true);

  PlayerCharacter->SetActorEnableCollision(true);

  LoadingWorld = false;
}

void AGameWorld::RenderChunks(FIntVector center, bool initialRender)
{
  if (center.Z < 0) return;

  // Coordinates of the chunk the player is within
  FIntVector chunkPosition = FIntVector(center.X / UConfig::CHUNK_SIZE, center.Y / UConfig::CHUNK_SIZE,
                                        center.Z / UConfig::CHUNK_SIZE);


  for (int32 x = chunkPosition.X - UConfig::CHUNK_RENDER_RADIUS; x <= chunkPosition.X + UConfig::CHUNK_RENDER_RADIUS; x
       ++)
  {
    for (int32 y = chunkPosition.Y - UConfig::CHUNK_RENDER_RADIUS; y <= chunkPosition.Y + UConfig::CHUNK_RENDER_RADIUS;
         y
         ++)
    {
      for (int32 z = 0; z <= UConfig::VERTICAL_CHUNKS; z++)
      {
        // Optimization: Render a sphere instead of a cube
        if (FVector::Distance((FVector(x, y, z) * UConfig::CHUNK_SIZE), FVector(center)) > UConfig::CHUNK_RENDER_RADIUS
          * UConfig::CHUNK_SIZE)
        {
          continue;
        }

        FIntVector location = FIntVector(x, y, z);

        if (ChunkStates.Contains(FIntVector(location)) && ChunkStates[FIntVector(location)].Get<0>() ==
          EChunkState::Rendered)
        {
          continue;
        }

        if (!ChunkStates.Contains(location)) continue;

        AChunk* chunk = ChunkPool->RetrieveChunk(); // Get a chunk from the pool
        if (initialRender)
        {
          chunk->Init(FVector(location) * UConfig::CHUNK_SIZE)->Render();
          chunk->SetActorLabel("Active at " + location.ToString());
          ChunkStates[location] = TTuple<EChunkState, AChunk*>(EChunkState::Rendered, chunk);
        }
        else
        {
          ChunksToRender.Enqueue(TTuple<AChunk*, FIntVector>(chunk, location));
        }
      }
    }
  }

  LastRenderCallPosition = chunkPosition;
}

void AGameWorld::LoadChunks(FIntVector center, bool initialLoad)
{
  FIntVector centerChunkCoordinates = FIntVector(center.X / UConfig::CHUNK_SIZE, center.Y / UConfig::CHUNK_SIZE,
                                                 center.Z / UConfig::CHUNK_SIZE);

  for (int32 x = centerChunkCoordinates.X - UConfig::CHUNK_LOAD_RADIUS; x <= centerChunkCoordinates.X +
       UConfig::CHUNK_LOAD_RADIUS; x++)
  {
    for (int32 y = centerChunkCoordinates.Y - UConfig::CHUNK_LOAD_RADIUS; y <= centerChunkCoordinates.Y +
         UConfig::CHUNK_LOAD_RADIUS; y++)
    {
      for (int32 z = 0; z <= UConfig::VERTICAL_CHUNKS; z++)
      {
        if (ChunkStates.Contains(FIntVector(x, y, z)) && (ChunkStates[FIntVector(x, y, z)].Get<0>() ==
          EChunkState::Generated ||
          ChunkStates[FIntVector(x, y, z)].Get<0>() == EChunkState::Rendered))
          continue;


        // if (FVector::Distance((FVector(x, y, z) * UConfig::CHUNK_SIZE), FVector(center)) > UConfig::CHUNK_LOAD_RADIUS
        //   * UConfig::CHUNK_SIZE)
        //   continue;
        if (!ChunkStates.Contains(FIntVector(x, y, z)))
        {
          if (initialLoad)
          {
            GenerateChunkData(FIntVector(x, y, z));
            ChunkStates.Add(FIntVector(x, y, z), TTuple<EChunkState, AChunk*>(EChunkState::Generated, nullptr));
          }
          else
          {
            ChunksToGenerateData.Enqueue(FIntVector(x, y, z));
          }

          // The chunks data are generated; an actor is not yet assigned 
        }
      }
    }
  }
}

void AGameWorld::LoadBiomes(FIntVector center, bool initialLoad)
{
  FIntVector centerChunkCoordinates = FIntVector(center.X / UConfig::CHUNK_SIZE, center.Y / UConfig::CHUNK_SIZE,
                                                 center.Z / UConfig::CHUNK_SIZE);


  TArray<TTask<TMap<FIntVector2, int32>>> tasks = TArray<TTask<TMap<FIntVector2, int32>>>();

  // TODO: Does it actually have IJobParallelFor alternative?

  FCriticalSection criticalSection;
  ParallelFor(2 * UConfig::BIOME_LOAD_RADIUS + 1, [&](int32 index)
  {
    int32 x = centerChunkCoordinates.X - UConfig::BIOME_LOAD_RADIUS + index;

    int32 z = 0;
    for (int32 y = centerChunkCoordinates.Y - UConfig::BIOME_LOAD_RADIUS; y <= centerChunkCoordinates.Y +
         UConfig::BIOME_LOAD_RADIUS; y++)
    {
      if (ChunkStates.Contains(FIntVector(x, y, z)) && (ChunkStates[FIntVector(x, y, z)].Get<0>() ==
        EChunkState::Generated ||
        ChunkStates[FIntVector(x, y, z)].Get<0>() == EChunkState::Rendered))
        continue;

      // if (FVector::Distance((FVector(x, y, z) * UConfig::CHUNK_SIZE), FVector(center)) > (
      //     UConfig::BIOME_LOAD_RADIUS +
      //     2)
      //   * UConfig::CHUNK_SIZE)
      //   continue;
      if (!ChunkStates.Contains(FIntVector(x, y, z)))
      {
        if (initialLoad)
        {
          TMap<FIntVector2, EBiome> result = GenerateBiomeMask(FIntVector(x, y, z) * UConfig::CHUNK_SIZE);
          criticalSection.Lock();
          BiomeMask.Append(result);
          criticalSection.Unlock();
        }
        else
        {
          criticalSection.Lock();
          ChunksToGenerateBiomes.Enqueue(FIntVector(x, y, z));
          criticalSection.Unlock();
        }
      }
    }
  });
}

TMap<FIntVector2, EBiome> AGameWorld::GenerateBiomeMask(FIntVector center)
{
  FIntVector2 chunkCoords = FIntVector2(center.X - center.X % UConfig::CHUNK_SIZE,
                                        center.Y - center.Y % UConfig::CHUNK_SIZE);
  int32 size = UConfig::CHUNK_SIZE;
  TMap<FIntVector2, int32> map = UBiomeGenerator::GenLayer::GetBiomeMap(chunkCoords.X, chunkCoords.Y, size,
                                                                        Seed);
  TMap<FIntVector2, EBiome> remapped = TMap<FIntVector2, EBiome>();
  for (auto result : map)
  {
    remapped.Add(FIntVector2(result.Key.X + chunkCoords.X, result.Key.Y + chunkCoords.Y),
                 static_cast<EBiome>(result.Value));
  }

  return remapped;
}


void AGameWorld::GenerateChunkData(FIntVector chunkCoordinates)
{
  FCriticalSection criticalSection;
  ParallelFor(UConfig::CHUNK_SIZE * UConfig::CHUNK_SIZE, [&](int32 index)
  {
    int32 x = index % UConfig::CHUNK_SIZE;
    int32 y = index / UConfig::CHUNK_SIZE;

    FVector2D point = FVector2D(x + chunkCoordinates.X * UConfig::CHUNK_SIZE,
                                y + chunkCoordinates.Y * UConfig::CHUNK_SIZE);


    // Biome blending
    FIntVector2 biomePoint = FIntVector2(point.X, point.Y);

    TMap<EBiome, double> biomes = TMap<EBiome, double>();

    EBiome middleBiome = BiomeMask.Contains(biomePoint) ? BiomeMask[biomePoint] : EBiome::Hell;
    for (int32 blendX = -UConfig::BIOME_BLEND_RADIUS; blendX <= UConfig::BIOME_BLEND_RADIUS; blendX++)
    {
      for (int32 blendY = -UConfig::BIOME_BLEND_RADIUS; blendY <= UConfig::BIOME_BLEND_RADIUS; blendY++)
      {
        FIntVector2 blendPoint = FIntVector2(biomePoint.X + blendX, biomePoint.Y + blendY);
        EBiome localBiome = BiomeMask.Contains(blendPoint) ? BiomeMask[blendPoint] : EBiome::Hell;

        if (biomes.Contains(localBiome)) biomes[localBiome] += UConfig::BIOME_BLEND_WEIGHT;
        else biomes.Add(localBiome, UConfig::BIOME_BLEND_WEIGHT);
      }
    }

    // if (biomes.Contains(EBiome::Plains))
    // {
    //   double weight = biomes[EBiome::Plains];
    //   biomes.Remove(EBiome::Plains);
    //   biomes.Add(EBiome::MesaRock, weight);
    // }

    int32 height = 0;
    for (TTuple<EBiome, double> biome : biomes)
    {
      height += Utility->GetHeightAtPoint(point, biome.Key) * biome.Value;
    }

    biomes.ValueSort([](double a, double b) { return a > b; });
    TArray<EBiome> biomeComposition;
    biomes.GetKeys(biomeComposition);
    EBiome prevailingBiome = biomeComposition[0];

    F3DArrayRef data = GenerateColumn(x, y, chunkCoordinates, height, prevailingBiome, biomes);

    criticalSection.Lock();
    WorldHeightMap.Add(biomePoint, height);
    WorldData.Append(data);
    criticalSection.Unlock();
  });

  BiomeRenderer->GenerateStructures(FIntVector(chunkCoordinates.X, chunkCoordinates.Y, 0));
}

F3DArrayRef AGameWorld::GenerateColumn(int32 x, int32 y, FIntVector chunkCoordinates, int32 height,
                                       EBiome prevailingBiome, const TMap<EBiome, double>& biomes)
{
  F3DArrayRef data = F3DArrayRef();
  for (int32 z = 0; z < UConfig::CHUNK_SIZE; z++)
  {
    FIntVector location = FIntVector(x, y, z) + chunkCoordinates * UConfig::CHUNK_SIZE;

    if (Changes.Contains(location))
    {
      data.Set(location, Changes.Get(location));
      continue;
    }

    if (location.Z <= height)
    {
      FVoxelRef block = BiomeRenderer->GetBlock(location.Z, height, prevailingBiome, biomes, location.X, location.Y);
      data.Set(location, block);
    }
    else if (height < UConfig::WATER_HEIGHT && location.Z < UConfig::WATER_HEIGHT)
    {
      // Some biomes are overriding water generation, handle those here
      if (UConfig::WATER_OVERRIDE_BIOMES.Contains(prevailingBiome))
      {
        FVoxelRef block = BiomeRenderer->GetWaterBlock(location.Z, height, prevailingBiome);
        data.Set(location, block);
      }
      else
      {
        data.Set(
          location, FVoxelRef(false, (int32)EBasicBlock::Water));
      }
    }
  }

  return data;
}


void AGameWorld::CleanupChunks()
{
  TArray<FIntVector> dispose = TArray<FIntVector>();
  FCriticalSection criticalSection;
  ParallelFor(ChunkPool->ActiveChunks.Num(), [&](int32 index)
  {
    AChunk* activeChunk = ChunkPool->ActiveChunks[index];
    FIntVector chunkCoordinates = FIntVector(activeChunk->WorldPosition * UConfig::CHUNK_SIZE_DIV);

    if ((ChunkStates.Contains(chunkCoordinates) && ChunkStates[chunkCoordinates].Get<0>() == EChunkState::Rendered))
    {
      if (ChunkStates[chunkCoordinates].Get<1>() == nullptr)
      {
        ChunkStates.Remove(chunkCoordinates);
      }
      else
      {
        FVector location = FVector(activeChunk->WorldPosition * 100);
        if (FVector::Distance(location, PlayerCharacter->GetActorLocation()) > UConfig::CHUNK_RENDER_RADIUS *
          UConfig::CHUNK_SIZE * 100)
        {
          criticalSection.Lock();
          dispose.Add(chunkCoordinates);
          criticalSection.Unlock();
        }
      }
    }
  });

  for (FIntVector chunkCoordinates : dispose)
  {
    AChunk* chunk = ChunkStates[chunkCoordinates].Get<1>();
    ChunkPool->ReturnChunk(chunk);
    ChunkStates[chunkCoordinates] = TTuple<EChunkState, AChunk*>(EChunkState::Generated, nullptr);
  }
}

void AGameWorld::RefreshChunkBlob(FIntVector chunkCoordinates)
{
  chunkCoordinates /= UConfig::CHUNK_SIZE;
  for (int32 x = -1; x <= 1; x++)
  {
    for (int32 y = -1; y <= 1; y++)
    {
      for (int32 z = -1; z <= 1; z++)
      {
        FIntVector location = chunkCoordinates + FIntVector(x, y, z);
        ChunkStates[location].Get<0>() = EChunkState::Reload;
      }
    }
  }

  for (auto element : ChunkStates)
  {
    if (element.Value.Get<0>() == EChunkState::Reload)
    {
      AChunk* oldChunk = element.Value.Get<1>();
      ChunkPool->ReturnChunk(oldChunk);
      AChunk* chunk = ChunkPool->RetrieveChunk(); // Get a chunk from the pool
      chunk->Init(FVector(element.Key) * UConfig::CHUNK_SIZE)->Render();
      chunk->SetActorLabel("Active at " + element.Key.ToString());
      ChunkStates[element.Key] = TTuple<EChunkState, AChunk*>(EChunkState::Rendered, chunk);
    }
  }
}

void AGameWorld::RemoveVoxel(FVector location)
{
  FIntVector voxelPosition = FIntVector(location.X / 100, location.Y / 100 - 1, location.Z / 100);

  if (WorldData.Get(voxelPosition).Color == (int32)EBasicBlock::Water) return;

  bool waterNearby = false;
  if (WorldData.Get(voxelPosition + FIntVector(0, 0, 1)).Color == (int32)EBasicBlock::Water) waterNearby = true;
  if (WorldData.Get(voxelPosition + FIntVector(1, 0, 0)).Color == (int32)EBasicBlock::Water) waterNearby = true;
  if (WorldData.Get(voxelPosition + FIntVector(0, 1, 0)).Color == (int32)EBasicBlock::Water) waterNearby = true;
  if (WorldData.Get(voxelPosition + FIntVector(-1, 0, 0)).Color == (int32)EBasicBlock::Water) waterNearby = true;
  if (WorldData.Get(voxelPosition + FIntVector(0, -1, 0)).Color == (int32)EBasicBlock::Water) waterNearby = true;

  // TODO: Check if the block is near water; if so, dont fill with air, but with water
  FVoxelRef replacement = waterNearby ? FVoxelRef(false, (int32)EBasicBlock::Water) : FVoxelRef(true, 0);
  WorldData.Set(voxelPosition, replacement);
  Changes.Set(voxelPosition, replacement);
  PendingChanges = true;


  FIntVector chunkPosition = FIntVector(voxelPosition.X / UConfig::CHUNK_SIZE * UConfig::CHUNK_SIZE,
                                        voxelPosition.Y / UConfig::CHUNK_SIZE * UConfig::CHUNK_SIZE,
                                        voxelPosition.Z / UConfig::CHUNK_SIZE * UConfig::CHUNK_SIZE);

  RefreshChunkBlob(chunkPosition);
}

void AGameWorld::TickGeneration()
{
  if (!ChunksToGenerateBiomes.IsEmpty())
  {
    for (int i = 0; i < UConfig::FRAME_LOAD_BATCH_SIZE; i++)
    {
      if (ChunksToGenerateBiomes.IsEmpty()) break;
      FIntVector location;
      ChunksToGenerateBiomes.Dequeue(location);
      TMap<FIntVector2, EBiome> result = GenerateBiomeMask(
        FIntVector(location.X, location.Y, location.Z) * UConfig::CHUNK_SIZE);

      BiomeMask.Append(result);
    }
  }

  if (!ChunksToGenerateData.IsEmpty())
  {
    for (int i = 0; i < UConfig::FRAME_LOAD_BATCH_SIZE; i++)
    {
      if (ChunksToGenerateData.IsEmpty()) break;
      FIntVector location;
      ChunksToGenerateData.Dequeue(location);
      GenerateChunkData(FIntVector(location.X, location.Y, location.Z));
      ChunkStates.Add(FIntVector(location.X, location.Y, location.Z),
                      TTuple<EChunkState, AChunk*>(EChunkState::Generated, nullptr));
    }
  }

  // if (!done) return;
  // if (!StructuresToSpawn.IsEmpty())
  // {
  //   done = false;
  //   for (int i = 0; i < UConfig::FRAME_LOAD_STRUCTURE_BATCH_SIZE; i++)
  //   {
  //     if (StructuresToSpawn.IsEmpty()) break;
  //     TTuple<EStructure, FIntVector, int64> tuple;
  //     StructuresToSpawn.Dequeue(tuple);
  //     UStructureGenerator::SpawnStructure(tuple.Get<0>(), tuple.Get<1>(), &tuple.Get<2>());
  //   }
  // }

  if (!ChunksToRender.IsEmpty())
  {
    for (int i = 0; i < UConfig::FRAME_LOAD_BATCH_SIZE; i++)
    {
      if (ChunksToRender.IsEmpty()) break;
      TTuple<AChunk*, FIntVector> tuple;
      ChunksToRender.Dequeue(tuple);
      if (ChunkStates[tuple.Get<1>()].Get<0>() != EChunkState::Generated)
      {
        ChunkPool->ReturnChunk(tuple.Get<0>());
        continue;
      }
      AChunk* chunk = tuple.Get<0>();
      FIntVector location = tuple.Get<1>();
      chunk->Init(FVector(location) * UConfig::CHUNK_SIZE)->Render();
      chunk->SetActorLabel("Active at " + location.ToString());
      ChunkStates[location] = TTuple<EChunkState, AChunk*>(EChunkState::Rendered, chunk);
    }
  }
}
