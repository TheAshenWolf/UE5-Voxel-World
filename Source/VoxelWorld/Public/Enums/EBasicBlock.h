#pragma once

UENUM(BlueprintType)
enum class EBasicBlock : uint8
{
  // This enum offers a hand-picked palette for the most common blocks.
  // Since all colors are written as IDs, it is better to have an enum
  UnrealRequirement = 0,
  Dirt = 16,
  GrassMild = 155,
  GrassJungle = 148,
  GrassDry = 168,
  GrassCold = 135,
  GrassFrozen = 128,
  GrassDamp = 157,
  Stone = 248,
  Sand = 190,
  Snow = 85,
  Wood = 235,
  Leaves = 143, // Also cactus. Deal with it.
  Sandstone = 192,
  Gravel = 250,
  Netherrack = 31,
  Water = 82,
  Ice = 111,
  Mycelium = 3,
  Podzol = 232,
  HardenedClay = 219,
  HardenedClayOrange = 207,
  HardenedClayBrown = 231,
  HardenedClayRed = 27,
  HardenedClayGray = 229,
  RedSand = 15,
  BirchWood = 247,
  RedMushroom = 24,
  BrownMushroom = 230
  
};