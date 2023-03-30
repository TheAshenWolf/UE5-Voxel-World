#pragma once
#include "EBiome.h"
#include "ETemperatureCategory.h"

#include "UBiomeGenerator.generated.h"
UCLASS()
class UBiomeGenerator : public UObject
{
  GENERATED_BODY()

public:
  /* > 2 */
  static inline const TArray<EBiome> WARM_BIOMES = {
    EBiome::Desert, EBiome::Desert, EBiome::Desert, EBiome::Savanna, EBiome::Savanna, EBiome::Plains
  };
  /* 1 - 2 */
  static inline const TArray<EBiome> MILD_BIOMES = {
    EBiome::Forest, EBiome::RoofedForest, EBiome::ExtremeHills, EBiome::Plains, EBiome::BirchForest, EBiome::Swampland
  };
  /* 0 - 1 */
  static inline const TArray<EBiome> COLD_BIOMES = {
    EBiome::Forest, EBiome::ExtremeHills, EBiome::Taiga, EBiome::Plains
  };
  /* < 0 */
  static inline const TArray<EBiome> FROZEN_BIOMES = {
    EBiome::IcePlains, EBiome::IcePlains, EBiome::IcePlains, EBiome::TaigaCold
  };
  static inline const TMap<EBiome, double> BIOME_TEMPERATURE = {
    {EBiome::Ocean, 0.5},
    {EBiome::Plains, 0.8},
    {EBiome::Desert, 2},
    {EBiome::ExtremeHills, 0.2},
    {EBiome::Forest, 0.8},
    {EBiome::Taiga, 0.2},
    {EBiome::Swampland, 0.8},
    {EBiome::River, 0.5},
    {EBiome::Hell, 2},
    {EBiome::Sky, 0.5},
    {EBiome::FrozenOcean, -0.5},
    {EBiome::FrozenRiver, -0.5},
    {EBiome::IcePlains, -0.7},
    {EBiome::IceMountains, -1},
    {EBiome::MushroomIsland, 0.9},
    {EBiome::MushroomIslandShore, 0.9},
    {EBiome::Beach, 0.8},
    {EBiome::DesertHills, 2},
    {EBiome::ForestHills, 0.7},
    {EBiome::TaigaHills, 0.25},
    {EBiome::SmallerExtremeHills, 0.2},
    {EBiome::Jungle, 0.95},
    {EBiome::JungleHills, 0.95},
    {EBiome::JungleEdge, 0.95},
    {EBiome::DeepOcean, 0.5},
    {EBiome::StoneBeach, 0.2},
    {EBiome::ColdBeach, 0.05},
    {EBiome::BirchForest, 0.6},
    {EBiome::BirchForestHills, 0.6},
    {EBiome::RoofedForest, 0.7},
    {EBiome::TaigaCold, -0.5},
    {EBiome::TaigaColdHills, -0.5},
    {EBiome::RedwoodTaiga, 0.3},
    {EBiome::RedwoodTaigaHills, 0.3},
    {EBiome::ExtremeHillsWithTrees, 0.2},
    {EBiome::Savanna, 1.2},
    {EBiome::SavannaRock, 1.0},
    {EBiome::Mesa, 2},
    {EBiome::MesaRock, 2},
    {EBiome::MesaClearRock, 2}
  };

  static ETemperatureCategory GetTemperatureCategory(int32 biome);
  static bool IsOceanic(int32 biome);
  static bool IsSnowy(int32 biome);

  class GenLayer
  {
  public:
    int64 BaseSeed;
    GenLayer* Parent;
    int64 WorldGeneratorSeed;
    int64 ChunkSeed;

    static TMap<FIntVector2, int32> GetBiomeMap(int32 x, int32 y, int32 size, int64 seed);
    virtual ~GenLayer() = default;
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent);
    virtual void InitWorldGenSeed(int64 seed);
    void InitChunkSeed(int32 x, int32 y); // Biomes span the whole height of the world; no need for z
    int32 NextInt(int64 modulator);
    EBiome SelectRandom(TArray<EBiome> array);
    int32 SelectRandom(TArray<int32> array);
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height);
  };

  class Island : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class Zoom : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual int32 BiomeOrRandom(int32 biome1, int32 biome2, int32 biome3, int32 biome4);
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class FuzzyZoom : public Zoom
  {
  public:
    virtual int32 BiomeOrRandom(int32 biome1, int32 biome2, int32 biome3, int32 biome4) override;
  };

  class AddIsland : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class ReduceOcean : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class AddSnow : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class EdgeColdWarm : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class EdgeFrozenWarm : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class EdgeSpecial : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class AddMushroomIsland : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class DeepOceanLayer : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class Biome : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class BiomeEdge : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
    bool CanBeNeighbors(int32 biome1, int32 biome2);
    // What is I? what is J? what is p_arr? what is new_arr?
    bool ReplaceBiomeEdge(TMap<FIntVector2, int32> parentArray, TMap<FIntVector2, int32>* newArr, int32 i, int32 j,
                          int32 targetBiome, int32 biomeToReplace, int32 replacingBiome);
    bool ReplaceBiomeEdgeIfNecessary(TMap<FIntVector2, int32> parentArray, TMap<FIntVector2, int32>* newArray, int32 i,
                                     int32 j, int32 targetBiome, int32 biomeToReplace, int32 replacingBiome);
  };

  class RiverInit : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class RiverLayer : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
    // No clue what "value" is
    int32 RiverFilter(int32 value);
  };

  class Hills : public GenLayer
  {
  public:
    GenLayer* Init(int64 baseSeed, GenLayer* parent, GenLayer* riverLayer);
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
    GenLayer* RiverLayer = nullptr;
  };

  class Smooth : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };

  class RiverMix : public GenLayer
  {
  public:
    GenLayer* Init(int64 baseSeed, GenLayer* parent, GenLayer* riverLayer);
    virtual void InitWorldGenSeed(int64 seed) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;

    GenLayer* RiverLayer = nullptr;
  };

  class Shore : public GenLayer
  {
  public:
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    bool IsMesa(int32 biome);
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
    bool IsJungleCompatible(int32 biome);
  };

  class VoronoiZoom : public GenLayer
  {
    virtual GenLayer* Init(int64 baseSeed, GenLayer* parent) override;
    virtual TMap<FIntVector2, int32> GetInts(int32 x, int32 y, int32 width, int32 height) override;
  };
};
