#pragma once
#include "BiomeGen/EBiome.h"

class UConfig
{
public:
  static constexpr int32 CHUNK_SIZE = 16;
  static constexpr float CHUNK_SIZE_DIV = 1.0f / CHUNK_SIZE;
  static constexpr int32 WORLD_HEIGHT = 128; // Has to be divisible by CHUNK_SIZE
  static constexpr int32 VERTICAL_CHUNKS = WORLD_HEIGHT / CHUNK_SIZE;
  static constexpr int32 CHUNK_RENDER_RADIUS = 8;
  static constexpr int32 CHUNK_LOAD_RADIUS = CHUNK_RENDER_RADIUS + 1;
  static constexpr int32 BIOME_LOAD_RADIUS = CHUNK_LOAD_RADIUS + 1;
  static constexpr int32 BIOME_BLEND_RADIUS = 3;
  static constexpr int32 SPAWN_DEVIATION = 256;
  // There is not really a point going higher than 3; more than that and the blending is barely noticeable and starts to be very heavy
  static constexpr double BIOME_BLEND_WEIGHT = 1.0 / ((BIOME_BLEND_RADIUS * 2.0 + 1.0) * (BIOME_BLEND_RADIUS * 2.0 +
    1.0)); // Calculation of the weight of each biome point in the blending

  // Batches
  static constexpr int32 FRAME_LOAD_BATCH_SIZE = 4;
  static constexpr int32 FRAME_LOAD_STRUCTURE_BATCH_SIZE = 16;


  // Worldgen
  // - Constant for the whole world
  static constexpr int64 WORLD_SEED_OVERRIDE = -1; // -1 for random
  static constexpr double OFFSET_X = 0.216781678628; // 0-1 - world offset along X axis
  static constexpr double OFFSET_Y = 0.67126813645; // 0-1 - world offset along Y axis
  static constexpr double CHUNK_REFRESH_RATE = 1; // seconds
  static constexpr double WATER_HEIGHT = 40;

  // - Per biome
  static constexpr double SMOOTH = 0.016138543; // lesser number - rolling hills, higher number - steep peaks
  static constexpr double HEIGHT_AMPLITUDE = 1;
  static constexpr bool GENERATE_STRUCTURES = true;
  static constexpr double TREE_CHANCE = 10; // 0-100 - chance of a tree spawning on a block


  // Pseudo-random number generator
  static constexpr double RNG_SMOOTH = 1;
  // lesser number - locations closer to each other produce numbers closer to each other, higher number - locations further from each other produce numbers closer to each other
  static constexpr double RNG_OFFSET_X = 0.7951675268; // 0-1 - world offset along X axis
  static constexpr double RNG_OFFSET_Y = 0.168768374; // 0-1 - world offset along Y axis



  // Additional Math
  static constexpr int32 CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;


  static inline const TArray<uint8> BIOME_COLORS = TArray<uint8>
  {
    89, // Ocean = 0,
    160, // Plains = 1,
    177, // Desert = 2,
    249, // ExtremeHills = 3,
    156, // Forest = 4,
    128, // Taiga = 5,
    170, // Swampland = 6,
    120, // River = 7,
    31, // Hell = 8,
    254, // Sky = 9,
    100, // FrozenOcean = 10,
    111, // FrozenRiver = 11,
    85, // IcePlains = 12,
    84, // IceMountains = 13,
    16, // MushroomIsland = 14,
    231, // MushroomIslandShore = 15,
    202, // Beaches = 16,
    180, // DesertHills = 17,
    157, // ForestHills = 18,
    129, // TaigaHills = 19,
    248, // SmallerExtremeHills = 20,
    143, // Jungle = 21,
    144, // JungleHills = 22,
    142, // JungleEdge = 23,
    79, // DeepOcean = 24,
    240, // StoneBeach = 25,
    247, // ColdBeach = 26,
    154, // BirchForest = 27,
    155, // BirchForestHills = 28,
    169, // RoofedForest = 29,
    117, // TaigaCold = 30,
    118, // TaigaColdHills = 31,
    131, // RedwoodTaiga = 32,
    132, // RedwoodTaigaHills = 33,
    153, // ExtremeHillsWithTrees = 34,
    183, // Savanna = 35,
    196, // SavannaRock = 36,
    220, // Mesa = 37,
    221, // MesaRock = 38,
    222, // MesaClearRock = 39
  };

  static const inline TMap<EBiome, double> BIOME_BASE_HEIGHT = {
    {EBiome::Ocean, 35},
    {EBiome::Plains, 47},
    {EBiome::Desert, 47},
    {EBiome::ExtremeHills, 65},
    {EBiome::Forest, 50},
    {EBiome::Taiga, 50},
    {EBiome::Swampland, 37},
    {EBiome::River, 36},
    {EBiome::Hell, 0},
    {EBiome::Sky, 128},
    {EBiome::FrozenOcean, 35},
    {EBiome::FrozenRiver, 36},
    {EBiome::IcePlains, 47},
    {EBiome::IceMountains, 60},
    {EBiome::MushroomIsland, 50},
    {EBiome::MushroomIslandShore, 47},
    {EBiome::Beach, 41},
    {EBiome::DesertHills, 55},
    {EBiome::ForestHills, 60},
    {EBiome::TaigaHills, 60},
    {EBiome::SmallerExtremeHills, 55},
    {EBiome::Jungle, 45},
    {EBiome::JungleHills, 55},
    {EBiome::JungleEdge, 45},
    {EBiome::DeepOcean, 25},
    {EBiome::StoneBeach, 45},
    {EBiome::ColdBeach, 45},
    {EBiome::BirchForest, 50},
    {EBiome::BirchForestHills, 60},
    {EBiome::RoofedForest, 50},
    {EBiome::TaigaCold, 50},
    {EBiome::TaigaColdHills, 60},
    {EBiome::RedwoodTaiga, 50},
    {EBiome::RedwoodTaigaHills, 60},
    {EBiome::ExtremeHillsWithTrees, 62},
    {EBiome::Savanna, 47},
    {EBiome::SavannaRock, 55},
    {EBiome::Mesa, 52},
    {EBiome::MesaRock, 65},
    {EBiome::MesaClearRock, 65}
  };

  // the max possible deviation from the average height
  static const inline TMap<EBiome, double> BIOME_DEVIATION = {
    {EBiome::Ocean, 4}, // {Ocean, 35},
    {EBiome::Plains, 6}, // {Plains, 47},
    {EBiome::Desert, 4}, // {Desert, 47},
    {EBiome::ExtremeHills, 25}, // {ExtremeHills, 65},
    {EBiome::Forest, 6}, // {Forest, 50},
    {EBiome::Taiga, 10}, // {Taiga, 50},
    {EBiome::Swampland, 6}, // {Swampland, 42},
    {EBiome::River, 4}, // {River, 37},
    {EBiome::Hell, 0}, // {Hell, 0},
    {EBiome::Sky, 0}, // {Sky, 0},
    {EBiome::FrozenOcean, 4}, // {FrozenOcean, 35},
    {EBiome::FrozenRiver, 4}, // {FrozenRiver, 37},
    {EBiome::IcePlains, 8}, // {IcePlains, 47},
    {EBiome::IceMountains, 10}, // {IceMountains, 60},
    {EBiome::MushroomIsland, 4}, // {MushroomIsland, 50},
    {EBiome::MushroomIslandShore, 4}, // {MushroomIslandShore, 47},
    {EBiome::Beach, 2}, // {Beaches, 47},
    {EBiome::DesertHills, 4}, // {DesertHills, 55},
    {EBiome::ForestHills, 6}, // {ForestHills, 60},
    {EBiome::TaigaHills, 10}, // {TaigaHills, 60},
    {EBiome::SmallerExtremeHills, 15}, // {SmallerExtremeHills, 55},
    {EBiome::Jungle, 6}, // {Jungle, 45},
    {EBiome::JungleHills, 6}, // {JungleHills, 55},
    {EBiome::JungleEdge, 6}, // {JungleEdge, 45},
    {EBiome::DeepOcean, 4}, // {DeepOcean, 25},
    {EBiome::StoneBeach, 4}, // {StoneBeach, 45},
    {EBiome::ColdBeach, 4}, // {ColdBeach, 45},
    {EBiome::BirchForest, 6}, // {BirchForest, 50},
    {EBiome::BirchForestHills, 6}, // {BirchForestHills, 60},
    {EBiome::RoofedForest, 6}, // {RoofedForest, 50},
    {EBiome::TaigaCold, 10}, // {TaigaCold, 50},
    {EBiome::TaigaColdHills, 10}, // {TaigaColdHills, 60},
    {EBiome::RedwoodTaiga, 10}, // {RedwoodTaiga, 50},
    {EBiome::RedwoodTaigaHills, 10}, // {RedwoodTaigaHills, 60},
    {EBiome::ExtremeHillsWithTrees, 15}, // {ExtremeHillsWithTrees, 62},
    {EBiome::Savanna, 4}, // {Savanna, 47},
    {EBiome::SavannaRock, 6}, // {SavannaRock, 55},
    {EBiome::Mesa, 4}, // {Mesa, 47},
    {EBiome::MesaRock, 6}, // {MesaRock, 55},
    {EBiome::MesaClearRock, 8} // {MesaClearRock, 55}
  };

  static const inline TArray<EBiome> WATER_OVERRIDE_BIOMES = {
    EBiome::FrozenOcean,
    EBiome::FrozenRiver,
    EBiome::ColdBeach
  };

  static const inline TArray<EBiome> COLUMN_GENERATION_BIOMES = {
    EBiome::Mesa,
    EBiome::MesaRock,
    EBiome::MesaClearRock
  };

  class BiomeSettings
  {
  public:
    static constexpr double BASIC_BLOCK_MISSING_CHANCE = 50; // 0-100 - chance of a block being missing in a structure, if it is marked as CanBeMissing

    
    // Chances are defined as 1 in [chance]
    static constexpr int32 OCEAN_SHIPWRECK_CHANCE = 16384;
    static constexpr int32 SHIPWRECK_MAST_CHANCE = 20; // percent
    static constexpr int32 PLAINS_TREE_CHANCE = 1024;
    static constexpr int32 CACTUS_CHANCE = 256;
    static constexpr int32 FOREST_TREE_CHANCE = 32;
    static constexpr int32 TAIGA_TREE_CHANCE = 64;
    static constexpr int32 BIRCH_FOREST_TREE_CHANCE = 64;
    static constexpr int32 REDWOOD_TAIGA_TREE_CHANCE = 256;
    static constexpr int32 JUNGLE_TREE_CHANCE = 16;
    static constexpr int32 MOUNTAIN_SHRUB_CHANCE = 256;
    static constexpr int32 MUSHROOM_ISLAND_SHROOM_CHANCE = 128;
    static constexpr int32 SAVANNA_TREE_CHANCE = 512;
    
  };
};
