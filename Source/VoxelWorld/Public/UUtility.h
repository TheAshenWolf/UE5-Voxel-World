#pragma once
#include "chrono"

#include "UNoise.h"
#include "BiomeGen/EBiome.h"
#include "Enums/EStructure.h"
#include "UUtility.generated.h"

UCLASS()
class UUtility : public UObject
{
  GENERATED_BODY()

  static inline UNoise* Noise = nullptr;

public:
  UUtility();
  static int32 GetHeightAtPoint(FVector2D point, EBiome biome);
  static double PseudoRandomForLocation(FVector2D point, double max);
  static int64 GetSeed();
  static void SetWorldSeed(int32 seed);

  // 0 to max - 1
  int32 PseudoRandomInt(FVector2D point, int32 max);
  static double FractalBrownianMotion(FVector2D point, int32 octaves, double lacunarity, double gain);
  static int64 GetChunkSeed(int32 x, int32 y);
  static int64 Convolute(int64 multiplier, int64 addition);
  static int32 NextInt(int64* chunkSeed, int64 modulator);
  static int32 GetHeight(int32 x, int32 y, EBiome biome);

  class Structures
  {
  public:
    // Loading
    static void LoadOaks();
    static void LoadSpruces();
    static void LoadAcacias();
    static void LoadJungles();
    static void LoadBirches();
    static void LoadMushrooms();

    // Selection
    static EStructure GetOak(int64* seed);

    inline static const TArray<EStructure> SMALL_SPRUCES = {EStructure::Spruce0, EStructure::Spruce1, EStructure::Spruce2, EStructure::Spruce3, EStructure::Spruce4, EStructure::Spruce5};
    inline static const int32 SMALL_SPRUCES_NUM = SMALL_SPRUCES.Num();
    static EStructure GetSmallSpruce(int64* seed);

    inline static const TArray<EStructure> LARGE_SPRUCES = {EStructure::SpruceLarge0, EStructure::SpruceLarge1};
    inline static const int32 LARGE_SPRUCES_NUM = LARGE_SPRUCES.Num();
    static EStructure GetLargeSpruce(int64* seed);

    inline static const TArray<EStructure> ACACIAS = {EStructure::Acacia0, EStructure::Acacia1, EStructure::Acacia2};
    inline static const int32 ACACIAS_NUM = ACACIAS.Num();
    static EStructure GetAcacia(int64* seed);
  };
};
