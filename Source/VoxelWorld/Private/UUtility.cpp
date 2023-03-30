#include "UUtility.h"

#include "AGameWorld.h"
#include "UConfig.h"
#include "UStructureGenerator.h"
#include "BiomeGen/EBiome.h"

UUtility::UUtility()
{
  Noise = NewObject<UNoise>();
}

int32 UUtility::GetHeightAtPoint(FVector2D point, EBiome biome)
{
  point *= UConfig::SMOOTH;
  double value = FractalBrownianMotion(point, 3, 1, 1); // 0-1
  int32 deviation = UConfig::BIOME_DEVIATION.Contains(biome) ? UConfig::BIOME_DEVIATION[biome] : 0;
  int32 baseHeight = UConfig::BIOME_BASE_HEIGHT.Contains(biome) ? UConfig::BIOME_BASE_HEIGHT[biome] : 50;
  value -= 0.5; // -0.5 - 0.5
  value *= deviation; // -deviation - +deviation
  value += baseHeight;

  return FMath::RoundToInt(value);
}

double UUtility::PseudoRandomForLocation(FVector2D point, double max)
{
  point *= UConfig::RNG_SMOOTH;
  float value = FMath::SRand();
  value = ((value + 1.0f) / 2.0f); // 0.0 to 1.0
  return value * max;
}

int64 UUtility::GetSeed()
{
  return Noise->GetSeed();
}

void UUtility::SetWorldSeed(int32 seed)
{
  if (seed != -1)
  {
    Noise->setNoiseSeed(seed);
  }
  else
  {
    FMath::RandInit(FDateTime::UtcNow().GetTicks());
    Noise->setNoiseSeed(FMath::RandRange(0, 2147483647 >> 1));
  }
}

int32 UUtility::PseudoRandomInt(FVector2D point, int32 max)
{
  point *= UConfig::RNG_SMOOTH * 0.9486576625;
  float value = FMath::SRand();
  return FMath::FloorToInt32(value * max);
}

// double UUtility::FractalBrownianMotion(FVector2D point, int32 octaves, double lacunarity, double gain)
// {
//   double total = 0;
//   double frequency = 1;
//   double amplitude = 1;
//   double maxValue = 0;
//   for (int32 i = 0; i < octaves; i++)
//   {
//     total += Noise->Eval(point.X * frequency, point.Y * frequency) * amplitude;
//     maxValue += amplitude;
//     amplitude *= gain;
//     frequency *= lacunarity;
//   }
//   return total / maxValue; // 0 to 1
// }

double UUtility::FractalBrownianMotion(FVector2D point, int32 octaves, double lacunarity, double gain)
{
  double value = Noise->GetSimplexNoise2D_EX(point.X, point.Y, lacunarity, gain, octaves, 1, true) / 2.0;
  value = FMath::Clamp(value, -1, 1);
  value = (value + 1) / 2.0;
  return value;
}

int64 UUtility::GetChunkSeed(int32 x, int32 y)
{
  int64 chunkSeed = GetSeed();
  chunkSeed = Convolute(chunkSeed, x);
  chunkSeed = Convolute(chunkSeed, y);
  chunkSeed = Convolute(chunkSeed, x);
  chunkSeed = Convolute(chunkSeed, y);
  return chunkSeed;
}

int64 UUtility::Convolute(int64 multiplier, int64 addition)
{
  return multiplier * (multiplier * 6364136223846793005 + 1442695040888963407) + addition;
}

int32 UUtility::NextInt(int64* chunkSeed, int64 modulator)
{
  int32 i = static_cast<int32>((*chunkSeed >> 24) % modulator);

  if (i < 0) i += modulator;

  *chunkSeed = Convolute(*chunkSeed, GetSeed());
  return i;
}

int32 UUtility::GetHeight(int32 x, int32 y, EBiome biome)
{
  if (!AGameWorld::WorldHeightMap.Contains(FIntVector2(x, y)))
  {
    AGameWorld::WorldHeightMap.Add(FIntVector2(x, y), GetHeightAtPoint(FVector2D(x, y), biome));
  }
  int32 z = AGameWorld::WorldHeightMap[FIntVector2(x, y)];
  return z;
}

void UUtility::Structures::LoadOaks()
{
  UStructureGenerator::LoadStructure(EStructure::Oak0, "oak/oak0");
  UStructureGenerator::LoadStructure(EStructure::OakBalloon0, "oak/oak_balloon0");
  UStructureGenerator::LoadStructure(EStructure::FancyOak0, "oak/fancy_oak0");
  UStructureGenerator::LoadStructure(EStructure::FancyOak1, "oak/fancy_oak1");
  UStructureGenerator::LoadStructure(EStructure::MountainShrubOak, "oak/mountain_shrub");
}

void UUtility::Structures::LoadSpruces()
{
  UStructureGenerator::LoadStructure(EStructure::Spruce0, "spruce/spruce0");
  UStructureGenerator::LoadStructure(EStructure::Spruce1, "spruce/spruce1");
  UStructureGenerator::LoadStructure(EStructure::Spruce2, "spruce/spruce2");
  UStructureGenerator::LoadStructure(EStructure::Spruce3, "spruce/spruce3");
  UStructureGenerator::LoadStructure(EStructure::Spruce4, "spruce/spruce4");
  UStructureGenerator::LoadStructure(EStructure::Spruce5, "spruce/spruce5");
  UStructureGenerator::LoadStructure(EStructure::SpruceLarge0, "spruce/spruce-large0");
  UStructureGenerator::LoadStructure(EStructure::SpruceLarge1, "spruce/spruce-large1");
}

void UUtility::Structures::LoadAcacias()
{
  UStructureGenerator::LoadStructure(EStructure::Acacia0, "acacia/acacia0");
  UStructureGenerator::LoadStructure(EStructure::Acacia1, "acacia/acacia1");
  UStructureGenerator::LoadStructure(EStructure::Acacia2, "acacia/acacia2");
}

void UUtility::Structures::LoadJungles()
{
  UStructureGenerator::LoadStructure(EStructure::JungleShrub, "jungle/jungle-shrub");
  UStructureGenerator::LoadStructure(EStructure::JungleSmall, "jungle/jungle-small");
  UStructureGenerator::LoadStructure(EStructure::JungleLarge, "jungle/jungle-large");
}

void UUtility::Structures::LoadBirches()
{
  UStructureGenerator::LoadStructure(EStructure::Birch, "birch/birch");
}

void UUtility::Structures::LoadMushrooms()
{
  UStructureGenerator::LoadStructure(EStructure::MushroomRed, "mushroom/mushroom-red");
  UStructureGenerator::LoadStructure(EStructure::MushroomBrown, "mushroom/mushroom-brown");
}

/// <summary>
/// Returns an oak variant
/// </summary>
EStructure UUtility::Structures::GetOak(int64* seed)
{
  int32 value = NextInt(seed, 83);
  
  value = FMath::Clamp(value, 0, 83);

  if (value == 0) return EStructure::FancyOak1; // the largest oak
  if (value <= 3) return EStructure::FancyOak0;
  if (value <= 9) return EStructure::OakBalloon0;
  
  return EStructure::Oak0;
}

/// <summary>
/// Returns a small spruce variant
/// </summary>
EStructure UUtility::Structures::GetSmallSpruce(int64* seed)
{
  int32 value = NextInt(seed, SMALL_SPRUCES_NUM);
  
  return SMALL_SPRUCES[value];
}

/// <summary>
/// Returns a small spruce variant
/// </summary>
EStructure UUtility::Structures::GetLargeSpruce(int64* seed)
{
  int32 value = NextInt(seed, LARGE_SPRUCES_NUM);
  
  return LARGE_SPRUCES[value];
}


/// <summary>
/// Returns a small spruce variant
/// </summary>
EStructure UUtility::Structures::GetAcacia(int64* seed)
{
  int32 value = NextInt(seed, ACACIAS_NUM);
  
  return ACACIAS[value];
}
