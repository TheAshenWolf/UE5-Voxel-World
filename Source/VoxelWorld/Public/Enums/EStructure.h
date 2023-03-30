#pragma once

UENUM(BlueprintType)
enum class EStructure : uint8
{
  // ocean, deepOcean
  Shipwreck = 0,
  // plains, forest, forest hills
  Oak0 = 1,
  OakBalloon0 = 2,
  FancyOak0 = 3,
  FancyOak1 = 4,
  // taiga, cold taiga, taiga hills, cold taiga hills, redwood, redwood hills
  Spruce0 = 5,
  Spruce1 = 6,
  Spruce2 = 7,
  Spruce3 = 8,
  Spruce4 = 9,
  Spruce5 = 10,
  SpruceLarge0 = 11,
  SpruceLarge1 = 12,
  // savanna
  Acacia0 = 13,
  Acacia1 = 14,
  Acacia2 = 15,
  // birch forest, birch forest hills
  Birch = 16,
  // mushroom island
  MushroomRed = 17,
  MushroomBrown = 18,
  // Hills
  MountainShrubOak = 19,
  // jungle
  JungleShrub = 20,
  JungleLarge = 21,
  JungleSmall = 22,
};
