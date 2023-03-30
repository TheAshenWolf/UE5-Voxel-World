#include "BiomeGen/UBiomeGenerator.h"

#include "UConfig.h"
#include "UUtility.h"

ETemperatureCategory UBiomeGenerator::GetTemperatureCategory(int32 biomeNum)
{
  EBiome biome = static_cast<EBiome>(biomeNum);
  double temperature = BIOME_TEMPERATURE[biome];

  if (temperature < 0) return Frozen;
  if (temperature < 0.2) return Cold;
  if (temperature < 1.0) return Mild;
  return Warm;
}

bool UBiomeGenerator::IsOceanic(int32 biome)
{
  return biome == (int32)EBiome::Ocean || biome == (int32)EBiome::DeepOcean || biome == (int32)EBiome::FrozenOcean;
}

bool UBiomeGenerator::IsSnowy(int32 biome)
{
  return biome == (int32)EBiome::FrozenOcean || biome == (int32)EBiome::FrozenRiver || biome == (int32)EBiome::IcePlains
    || biome ==
    (int32)EBiome::IceMountains || biome ==
    (int32)EBiome::ColdBeach || biome == (int32)EBiome::TaigaCold || biome == (int32)EBiome::TaigaColdHills;
}

//  ██████╗ ███████╗███╗   ██╗██╗      █████╗ ██╗   ██╗███████╗██████╗ 
// ██╔════╝ ██╔════╝████╗  ██║██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗
// ██║  ███╗█████╗  ██╔██╗ ██║██║     ███████║ ╚████╔╝ █████╗  ██████╔╝
// ██║   ██║██╔══╝  ██║╚██╗██║██║     ██╔══██║  ╚██╔╝  ██╔══╝  ██╔══██╗
// ╚██████╔╝███████╗██║ ╚████║███████╗██║  ██║   ██║   ███████╗██║  ██║
// ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝

// Works roughly like this
// https://preview.redd.it/j5kahu83piv51.gif?format=mp4&v=enabled&s=f30aa370e15b25aba02dfa2c611c089bae3bbe45

TMap<FIntVector2, int32> UBiomeGenerator::GenLayer::GetBiomeMap(int32 x, int32 y, int32 size, int64 seed)
{
  GenLayer* island = Island().Init(1, nullptr);
  GenLayer* fuzzyZoom = FuzzyZoom().Init(2000, island);
  GenLayer* addIsland1 = AddIsland().Init(1, fuzzyZoom);
  GenLayer* zoom1 = Zoom().Init(2001, addIsland1);
  GenLayer* addIsland2 = AddIsland().Init(2, zoom1);
  addIsland2 = AddIsland().Init(50, addIsland2);
  addIsland2 = AddIsland().Init(70, addIsland2);
  GenLayer* reduceOcean = ReduceOcean().Init(2, addIsland2);
  GenLayer* addSnow = AddSnow().Init(2, reduceOcean);
  GenLayer* addIsland3 = AddIsland().Init(3, addSnow);
  GenLayer* edgeColdWarm = EdgeColdWarm().Init(2, addIsland3);
  GenLayer* edgeFrozenWarm = EdgeFrozenWarm().Init(2, edgeColdWarm);
  GenLayer* edgeSpecial = EdgeSpecial().Init(3, edgeFrozenWarm);
  GenLayer* zoom2 = Zoom().Init(2002, edgeSpecial);
  zoom2 = Zoom().Init(2003, zoom2); // +
  GenLayer* addIsland4 = AddIsland().Init(4, zoom2);
  GenLayer* mushroom = AddMushroomIsland().Init(5, addIsland4);
  GenLayer* deepOcean = DeepOceanLayer().Init(4, mushroom);
  GenLayer* biome = Biome().Init(200, deepOcean); // -
  GenLayer* riverInit = RiverInit().Init(100, deepOcean); // <- ?
  GenLayer* riverZoom1 = Zoom().Init(1000, riverInit);
  riverZoom1 = Zoom().Init(1001, riverZoom1); // -
  GenLayer* zoom3 = Zoom().Init(1000, biome);
  zoom3 = Zoom().Init(1001, zoom3);
  GenLayer* biomeEdge = BiomeEdge().Init(1000, zoom3);
  GenLayer* hills = Hills().Init(1000, biomeEdge, riverZoom1);
  GenLayer* riverZoom2 = Zoom().Init(1000, riverZoom1);
  riverZoom2 = Zoom().Init(1001, riverZoom2);
  riverZoom2 = Zoom().Init(1002, riverZoom2);
  riverZoom2 = Zoom().Init(1003, riverZoom2);
  GenLayer* zoom4 = Zoom().Init(1000, hills);
  GenLayer* addIsland5 = AddIsland().Init(3, zoom4);
  GenLayer* zoom5 = Zoom().Init(1001, addIsland5);
  GenLayer* shore = Shore().Init(1000, zoom5);
  GenLayer* zoom6 = Zoom().Init(1002, shore);
  zoom6 = Zoom().Init(1003, zoom6);
  GenLayer* smooth = Smooth().Init(1000, zoom6);
  GenLayer* river = RiverLayer().Init(1, riverZoom2);
  GenLayer* riverSmooth = Smooth().Init(1000, river);
  GenLayer* riverMix = RiverMix().Init(100, smooth, riverSmooth);
  GenLayer* notVoronoiZoom = Zoom().Init(1000, riverMix);
  notVoronoiZoom = Zoom().Init(1001, notVoronoiZoom);

  notVoronoiZoom->InitWorldGenSeed(seed);

  int32 width = size;
  int32 height = size;
  return notVoronoiZoom->GetInts(x, y, width, height);
}

UBiomeGenerator::GenLayer* UBiomeGenerator::GenLayer::Init(int64 baseSeed, GenLayer* parent)
{
  BaseSeed = baseSeed;
  BaseSeed = UUtility::Convolute(BaseSeed, baseSeed);
  BaseSeed = UUtility::Convolute(BaseSeed, baseSeed);
  BaseSeed = UUtility::Convolute(BaseSeed, baseSeed);

  WorldGeneratorSeed = 0;
  Parent = nullptr;
  ChunkSeed = 0;

  return this;
}

void UBiomeGenerator::GenLayer::InitWorldGenSeed(int64 seed)
{
  WorldGeneratorSeed = seed;

  if (Parent != nullptr)
  {
    Parent->InitWorldGenSeed(seed);
  }

  WorldGeneratorSeed = UUtility::Convolute(WorldGeneratorSeed, BaseSeed);
  WorldGeneratorSeed = UUtility::Convolute(WorldGeneratorSeed, BaseSeed);
  WorldGeneratorSeed = UUtility::Convolute(WorldGeneratorSeed, BaseSeed);
}

void UBiomeGenerator::GenLayer::InitChunkSeed(int32 x, int32 y)
{
  ChunkSeed = WorldGeneratorSeed;
  ChunkSeed = UUtility::Convolute(ChunkSeed, x);
  ChunkSeed = UUtility::Convolute(ChunkSeed, y);
  ChunkSeed = UUtility::Convolute(ChunkSeed, x);
  ChunkSeed = UUtility::Convolute(ChunkSeed, y);
}

int32 UBiomeGenerator::GenLayer::NextInt(int64 modulator)
{
  int32 i = static_cast<int32>((ChunkSeed >> 24) % modulator);

  if (i < 0) i += modulator;

  ChunkSeed = UUtility::Convolute(ChunkSeed, WorldGeneratorSeed);
  return i;
}

EBiome UBiomeGenerator::GenLayer::SelectRandom(TArray<EBiome> array)
{
  return array[NextInt(array.Num())];
}

int32 UBiomeGenerator::GenLayer::SelectRandom(TArray<int32> array)
{
  return array[NextInt(array.Num())];
}

TMap<FIntVector2, int32> UBiomeGenerator::GenLayer::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  // This should never happen, as you should use overrides
  return TMap<FIntVector2, int32>();
}

bool BiomesEqualOrMesaPlateau(EBiome biome1, EBiome biome2)
{
  if (biome1 == biome2) return true;
  if (biome1 == EBiome::MesaRock || biome1 == EBiome::MesaClearRock)
    return biome2 == EBiome::MesaRock || biome2 ==
      EBiome::MesaClearRock;
  return false;
}

// ██╗███████╗██╗      █████╗ ███╗   ██╗██████╗ 
// ██║██╔════╝██║     ██╔══██╗████╗  ██║██╔══██╗
// ██║███████╗██║     ███████║██╔██╗ ██║██║  ██║
// ██║╚════██║██║     ██╔══██║██║╚██╗██║██║  ██║
// ██║███████║███████╗██║  ██║██║ ╚████║██████╔╝
// ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ 

UBiomeGenerator::GenLayer* UBiomeGenerator::Island::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::Island::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();

  for (int32 i = 0; i < width; i++)
  {
    for (int32 j = 0; j < height; j++)
    {
      InitChunkSeed(x + i, y + j);
      newArr.Add(FIntVector2(i, j), NextInt(10) == 0 ? (int32)EBiome::Plains : (int32)EBiome::Ocean);
    }
  }

  if (-width < x && x <= 0 && (-height < y && y <= 0))
  {
    newArr.Add(FIntVector2(-x, -y), (int32)EBiome::Plains);
  }

  return newArr;
}

//
// ███████╗ ██████╗  ██████╗ ███╗   ███╗
// ╚══███╔╝██╔═══██╗██╔═══██╗████╗ ████║
//   ███╔╝ ██║   ██║██║   ██║██╔████╔██║
//  ███╔╝  ██║   ██║██║   ██║██║╚██╔╝██║
// ███████╗╚██████╔╝╚██████╔╝██║ ╚═╝ ██║
// ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝     ╚═╝

UBiomeGenerator::GenLayer* UBiomeGenerator::Zoom::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

int32 UBiomeGenerator::Zoom::BiomeOrRandom(int32 biome1, int32 biome2, int32 biome3, int32 biome4)
{
  // Original implementation
  if (biome1 == biome2 && biome2 == biome3) return biome1;
  if (biome1 == biome2 && biome2 == biome4) return biome1;
  if (biome1 == biome3 && biome3 == biome4) return biome1;
  if (biome1 == biome2 && biome3 != biome4) return biome1;
  if (biome1 == biome3 && biome2 != biome4) return biome1;
  if (biome1 == biome4 && biome2 != biome3) return biome1;

  if (biome2 == biome3 && biome3 == biome4) return biome2;
  if (biome2 == biome3 && biome1 != biome4) return biome2;
  if (biome2 == biome4 && biome1 != biome3) return biome2;

  if (biome3 == biome4 && biome1 != biome2) return biome3;

  return SelectRandom(TArray{biome1, biome2, biome3, biome4});
}

TMap<FIntVector2, int32> UBiomeGenerator::Zoom::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x >> 1;
  int32 parentY = y >> 1;
  int32 parentWidth = (width >> 1) + 2;
  int32 parentHeight = (height >> 1) + 2;
  TMap<FIntVector2, int32> parentInts = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);
  // int32 iWidth = (pWidth - 1) << 1;
  // int32 iHeight = (pHeight - 1) << 1;

  TMap<FIntVector2, int32> iArray = TMap<FIntVector2, int32>();
  for (int32 j = 0; j < parentHeight - 1; j++)
  {
    for (int32 i = 0; i < parentWidth - 1; i++)
    {
      InitChunkSeed((parentX + i) << 1, (parentY + j) << 1);
      iArray.Add(FIntVector2(2 * i, 2 * j), parentInts[FIntVector2(i, j)]);
      iArray.Add(FIntVector2(2 * i, 2 * j + 1), SelectRandom(TArray<int32>{
                   parentInts[FIntVector2(i, j)], parentInts[FIntVector2(i, j + 1)]
                 }));
      iArray.Add(FIntVector2(2 * i + 1, 2 * j), SelectRandom(TArray<int32>{
                   parentInts[FIntVector2(i, j)], parentInts[FIntVector2(i + 1, j)]
                 }));
      iArray.Add(FIntVector2(2 * i + 1, 2 * j + 1), BiomeOrRandom(parentInts[FIntVector2(i, j)],
                                                                  parentInts[FIntVector2(i + 1, j)],
                                                                  parentInts[FIntVector2(i, j + 1)],
                                                                  parentInts[FIntVector2(i + 1, j + 1)]));
    }
  }

  int32 startX = x & 1;
  int32 startY = y & 1;
  // IMPORTANT: Line 129 in GenLayers.py > new_arr = i_arr[startx:(startx + width), starty:(starty + height)]
  // IMPORTANT: "Take all elements from the array from startx to startx + width and from starty to starty + height and put them into a new array"
  for (int32 i = 0; i < width; i++)
  {
    for (int32 j = 0; j < height; j++)
    {
      newArr.Add(FIntVector2(i, j), iArray[FIntVector2(startX + i, startY + j)]);
    }
  }

  return newArr;
}

// ███████╗██╗   ██╗███████╗███████╗██╗   ██╗    ███████╗ ██████╗  ██████╗ ███╗   ███╗
// ██╔════╝██║   ██║╚══███╔╝╚══███╔╝╚██╗ ██╔╝    ╚══███╔╝██╔═══██╗██╔═══██╗████╗ ████║
// █████╗  ██║   ██║  ███╔╝   ███╔╝  ╚████╔╝       ███╔╝ ██║   ██║██║   ██║██╔████╔██║
// ██╔══╝  ██║   ██║ ███╔╝   ███╔╝    ╚██╔╝       ███╔╝  ██║   ██║██║   ██║██║╚██╔╝██║
// ██║     ╚██████╔╝███████╗███████╗   ██║       ███████╗╚██████╔╝╚██████╔╝██║ ╚═╝ ██║
// ╚═╝      ╚═════╝ ╚══════╝╚══════╝   ╚═╝       ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝     ╚═╝


int32 UBiomeGenerator::FuzzyZoom::BiomeOrRandom(int32 biome1, int32 biome2, int32 biome3, int32 biome4)
{
  return SelectRandom(TArray{biome1, biome2, biome3, biome4});
}

//  █████╗ ██████╗ ██████╗     ██╗███████╗██╗      █████╗ ███╗   ██╗██████╗ 
// ██╔══██╗██╔══██╗██╔══██╗    ██║██╔════╝██║     ██╔══██╗████╗  ██║██╔══██╗
// ███████║██║  ██║██║  ██║    ██║███████╗██║     ███████║██╔██╗ ██║██║  ██║
// ██╔══██║██║  ██║██║  ██║    ██║╚════██║██║     ██╔══██║██║╚██╗██║██║  ██║
// ██║  ██║██████╔╝██████╔╝    ██║███████║███████╗██║  ██║██║ ╚████║██████╔╝
// ╚═╝  ╚═╝╚═════╝ ╚═════╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ 
//                                                                          


UBiomeGenerator::GenLayer* UBiomeGenerator::AddIsland::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::AddIsland::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 i = 0; i < width; i++)
  {
    for (int32 j = 0; j < height; j++)
    {
      InitChunkSeed(x + i, y + j);
      // Whoever wrote this should hang in hell...
      int32 a = parentArray[FIntVector2(i, j)];
      int32 b = parentArray[FIntVector2(i + 2, j)];
      int32 c = parentArray[FIntVector2(i, j + 2)];
      int32 d = parentArray[FIntVector2(i + 2, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (k != (int32)EBiome::Ocean || (a == (int32)EBiome::Ocean && b == (int32)EBiome::Ocean && c == (int32)
        EBiome::Ocean && d == (int32)EBiome::Ocean))
      {
        if (k > 0 && (a == (int32)EBiome::Ocean && b == (int32)EBiome::Ocean && c == (int32)EBiome::Ocean && d == (
          int32)EBiome::Ocean))
        {
          if (NextInt(5) == 0)
          {
            if (k == (int32)EBiome::Forest)
            {
              newArr.Add(FIntVector2(i, j), (int32)EBiome::Forest);
            }
            else
            {
              newArr.Add(FIntVector2(i, j), (int32)EBiome::Ocean);
            }
          }
          else
          {
            newArr.Add(FIntVector2(i, j), k);
          }
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
      else
      {
        int32 l2 = 1;
        int32 n = 1;
        if (a != (int32)EBiome::Ocean)
        {
          if (NextInt(l2) == 0) n = a;
          l2++;
        }
        if (b != (int32)EBiome::Ocean)
        {
          if (NextInt(l2) == 0) n = b;
          l2++;
        }
        if (c != (int32)EBiome::Ocean)
        {
          if (NextInt(l2) == 0) n = c;
          l2++;
        }
        if (d != (int32)EBiome::Ocean)
        {
          if (NextInt(l2) == 0) n = d;
        }

        if (NextInt(3) == 0)
        {
          newArr.Add(FIntVector2(i, j), n);
        }
        else if (n == 4) newArr.Add(FIntVector2(i, j), (int32)EBiome::Forest);
        else newArr.Add(FIntVector2(i, j), (int32)EBiome::Ocean);
      }
    }
  }

  return newArr;
}

// ██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗███████╗     ██████╗  ██████╗███████╗ █████╗ ███╗   ██╗
// ██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝██╔════╝    ██╔═══██╗██╔════╝██╔════╝██╔══██╗████╗  ██║
// ██████╔╝█████╗  ██║  ██║██║   ██║██║     █████╗      ██║   ██║██║     █████╗  ███████║██╔██╗ ██║
// ██╔══██╗██╔══╝  ██║  ██║██║   ██║██║     ██╔══╝      ██║   ██║██║     ██╔══╝  ██╔══██║██║╚██╗██║
// ██║  ██║███████╗██████╔╝╚██████╔╝╚██████╗███████╗    ╚██████╔╝╚██████╗███████╗██║  ██║██║ ╚████║
// ╚═╝  ╚═╝╚══════╝╚═════╝  ╚═════╝  ╚═════╝╚══════╝     ╚═════╝  ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝


UBiomeGenerator::GenLayer* UBiomeGenerator::ReduceOcean::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;
  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::ReduceOcean::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 a = parentArray[FIntVector2(i + 1, j)];
      int32 b = parentArray[FIntVector2(i + 2, j + 1)];
      int32 c = parentArray[FIntVector2(i, j + 1)];
      int32 d = parentArray[FIntVector2(i + 1, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      bool allZeros = (k == a && k == b && k == c && k == d && k == 0 && NextInt(2) == 0);
      newArr.Add(FIntVector2(i, j), allZeros ? (int32)EBiome::Plains : k);
    }
  }

  return newArr;
}


//  █████╗ ██████╗ ██████╗     ███████╗███╗   ██╗ ██████╗ ██╗    ██╗
// ██╔══██╗██╔══██╗██╔══██╗    ██╔════╝████╗  ██║██╔═══██╗██║    ██║
// ███████║██║  ██║██║  ██║    ███████╗██╔██╗ ██║██║   ██║██║ █╗ ██║
// ██╔══██║██║  ██║██║  ██║    ╚════██║██║╚██╗██║██║   ██║██║███╗██║
// ██║  ██║██████╔╝██████╔╝    ███████║██║ ╚████║╚██████╔╝╚███╔███╔╝
// ╚═╝  ╚═╝╚═════╝ ╚═════╝     ╚══════╝╚═╝  ╚═══╝ ╚═════╝  ╚══╝╚══╝ 


UBiomeGenerator::GenLayer* UBiomeGenerator::AddSnow::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::AddSnow::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(x, y, width, height);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 k = parentArray[FIntVector2(i, j)];
      if (k == (int32)EBiome::Ocean)
      {
        newArr.Add(FIntVector2(i, j), (int32)EBiome::Ocean);
      }
      else
      {
        int32 r = NextInt(6);
        if (r == 0) r = 4;
        else if (r <= 1) r = 3;
        else r = 1;
        newArr.Add(FIntVector2(i, j), r);
      }
    }
  }

  return newArr;
}

// ███████╗██████╗  ██████╗ ███████╗     ██████╗ ██████╗ ██╗     ██████╗     ██╗    ██╗ █████╗ ██████╗ ███╗   ███╗
// ██╔════╝██╔══██╗██╔════╝ ██╔════╝    ██╔════╝██╔═══██╗██║     ██╔══██╗    ██║    ██║██╔══██╗██╔══██╗████╗ ████║
// █████╗  ██║  ██║██║  ███╗█████╗      ██║     ██║   ██║██║     ██║  ██║    ██║ █╗ ██║███████║██████╔╝██╔████╔██║
// ██╔══╝  ██║  ██║██║   ██║██╔══╝      ██║     ██║   ██║██║     ██║  ██║    ██║███╗██║██╔══██║██╔══██╗██║╚██╔╝██║
// ███████╗██████╔╝╚██████╔╝███████╗    ╚██████╗╚██████╔╝███████╗██████╔╝    ╚███╔███╔╝██║  ██║██║  ██║██║ ╚═╝ ██║
// ╚══════╝╚═════╝  ╚═════╝ ╚══════╝     ╚═════╝ ╚═════╝ ╚══════╝╚═════╝      ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::EdgeColdWarm::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::EdgeColdWarm::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (k == 1)
      {
        int32 a = parentArray[FIntVector2(i + 1, j)];
        int32 b = parentArray[FIntVector2(i + 2, j + 1)];
        int32 c = parentArray[FIntVector2(i, j + 1)];
        int32 d = parentArray[FIntVector2(i + 1, j + 2)];
        bool flag1 = a == 3 || b == 3 || c == 3 || d == 3;
        bool flag2 = a == 4 || b == 4 || c == 4 || d == 4;
        if (flag1 || flag2)
        {
          k = 2;
        }
      }
      newArr.Add(FIntVector2(i, j), k);
    }
  }

  return newArr;
}


// ███████╗██████╗  ██████╗ ███████╗    ███████╗██████╗  ██████╗ ███████╗███████╗███╗   ██╗    ██╗    ██╗ █████╗ ██████╗ ███╗   ███╗
// ██╔════╝██╔══██╗██╔════╝ ██╔════╝    ██╔════╝██╔══██╗██╔═══██╗╚══███╔╝██╔════╝████╗  ██║    ██║    ██║██╔══██╗██╔══██╗████╗ ████║
// █████╗  ██║  ██║██║  ███╗█████╗      █████╗  ██████╔╝██║   ██║  ███╔╝ █████╗  ██╔██╗ ██║    ██║ █╗ ██║███████║██████╔╝██╔████╔██║
// ██╔══╝  ██║  ██║██║   ██║██╔══╝      ██╔══╝  ██╔══██╗██║   ██║ ███╔╝  ██╔══╝  ██║╚██╗██║    ██║███╗██║██╔══██║██╔══██╗██║╚██╔╝██║
// ███████╗██████╔╝╚██████╔╝███████╗    ██║     ██║  ██║╚██████╔╝███████╗███████╗██║ ╚████║    ╚███╔███╔╝██║  ██║██║  ██║██║ ╚═╝ ██║
// ╚══════╝╚═════╝  ╚═════╝ ╚══════╝    ╚═╝     ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═══╝     ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::EdgeFrozenWarm::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::EdgeFrozenWarm::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (k == 4)
      {
        int32 a = parentArray[FIntVector2(i + 1, j)];
        int32 b = parentArray[FIntVector2(i + 2, j + 1)];
        int32 c = parentArray[FIntVector2(i, j + 1)];
        int32 d = parentArray[FIntVector2(i + 1, j + 2)];
        bool flag1 = a == 2 || b == 2 || c == 2 || d == 2;
        bool flag2 = a == 1 || b == 1 || c == 1 || d == 1;
        if (flag1 || flag2)
        {
          k = 3;
        }
      }
      newArr.Add(FIntVector2(i, j), k);
    }
  }

  return newArr;
}


// ███████╗██████╗  ██████╗ ███████╗    ███████╗██████╗ ███████╗ ██████╗██╗ █████╗ ██╗     
// ██╔════╝██╔══██╗██╔════╝ ██╔════╝    ██╔════╝██╔══██╗██╔════╝██╔════╝██║██╔══██╗██║     
// █████╗  ██║  ██║██║  ███╗█████╗      ███████╗██████╔╝█████╗  ██║     ██║███████║██║     
// ██╔══╝  ██║  ██║██║   ██║██╔══╝      ╚════██║██╔═══╝ ██╔══╝  ██║     ██║██╔══██║██║     
// ███████╗██████╔╝╚██████╔╝███████╗    ███████║██║     ███████╗╚██████╗██║██║  ██║███████╗
// ╚══════╝╚═════╝  ╚═════╝ ╚══════╝    ╚══════╝╚═╝     ╚══════╝ ╚═════╝╚═╝╚═╝  ╚═╝╚══════╝


UBiomeGenerator::GenLayer* UBiomeGenerator::EdgeSpecial::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::EdgeSpecial::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(x, y, width, height);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 k = parentArray[FIntVector2(i, j)];
      if (k != 0 && NextInt(13) == 0)
      {
        k = k | (((1 + NextInt(15)) << 8) & 3840);
      }
      newArr.Add(FIntVector2(i, j), k);
    }
  }

  return newArr;
}

// ███╗   ███╗██╗   ██╗███████╗██╗  ██╗██████╗  ██████╗  ██████╗ ███╗   ███╗
// ████╗ ████║██║   ██║██╔════╝██║  ██║██╔══██╗██╔═══██╗██╔═══██╗████╗ ████║
// ██╔████╔██║██║   ██║███████╗███████║██████╔╝██║   ██║██║   ██║██╔████╔██║
// ██║╚██╔╝██║██║   ██║╚════██║██╔══██║██╔══██╗██║   ██║██║   ██║██║╚██╔╝██║
// ██║ ╚═╝ ██║╚██████╔╝███████║██║  ██║██║  ██║╚██████╔╝╚██████╔╝██║ ╚═╝ ██║
// ╚═╝     ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚═╝     ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::AddMushroomIsland::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::AddMushroomIsland::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 a = parentArray[FIntVector2(i, j)];
      int32 b = parentArray[FIntVector2(i + 2, j)];
      int32 c = parentArray[FIntVector2(i, j + 2)];
      int32 d = parentArray[FIntVector2(i + 2, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (a == 0 && b == 0 && c == 0 && d == 0 && NextInt(100) == 0)
      {
        newArr.Add(FIntVector2(i, j), (int32)EBiome::MushroomIsland);
      }
      else
      {
        newArr.Add(FIntVector2(i, j), k);
      }
    }
  }

  return newArr;
}


// ██████╗ ███████╗███████╗██████╗      ██████╗  ██████╗███████╗ █████╗ ███╗   ██╗
// ██╔══██╗██╔════╝██╔════╝██╔══██╗    ██╔═══██╗██╔════╝██╔════╝██╔══██╗████╗  ██║
// ██║  ██║█████╗  █████╗  ██████╔╝    ██║   ██║██║     █████╗  ███████║██╔██╗ ██║
// ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝     ██║   ██║██║     ██╔══╝  ██╔══██║██║╚██╗██║
// ██████╔╝███████╗███████╗██║         ╚██████╔╝╚██████╗███████╗██║  ██║██║ ╚████║
// ╚═════╝ ╚══════╝╚══════╝╚═╝          ╚═════╝  ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝


UBiomeGenerator::GenLayer* UBiomeGenerator::DeepOceanLayer::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::DeepOceanLayer::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 a = parentArray[FIntVector2(i + 1, j)];
      int32 b = parentArray[FIntVector2(i + 2, j + 1)];
      int32 c = parentArray[FIntVector2(i, j + 1)];
      int32 d = parentArray[FIntVector2(i + 1, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (a == 0 && b == 0 && c == 0 && d == 0 && k == 0)
      {
        newArr.Add(FIntVector2(i, j), (int32)EBiome::DeepOcean);
      }
      else
      {
        newArr.Add(FIntVector2(i, j), k);
      }
    }
  }

  return newArr;
}

// ██████╗ ██╗ ██████╗ ███╗   ███╗███████╗
// ██╔══██╗██║██╔═══██╗████╗ ████║██╔════╝
// ██████╔╝██║██║   ██║██╔████╔██║█████╗  
// ██╔══██╗██║██║   ██║██║╚██╔╝██║██╔══╝  
// ██████╔╝██║╚██████╔╝██║ ╚═╝ ██║███████╗
// ╚═════╝ ╚═╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝


UBiomeGenerator::GenLayer* UBiomeGenerator::Biome::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::Biome::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(x, y, width, height);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 k = parentArray[FIntVector2(i, j)];
      int32 r = (k & 3840) >> 8;
      k = k & -3841;

      if (k == 0 || k == (int32)EBiome::DeepOcean || k == (int32)EBiome::FrozenOcean)
      {
        newArr.Add(FIntVector2(i, j), k);
      }
      else if (k == 1)
      {
        newArr.Add(FIntVector2(i, j), (int32)SelectRandom(WARM_BIOMES));
      }
      else if (k == 2)
      {
        if (r > 2)
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::Jungle);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), (int32)SelectRandom(MILD_BIOMES));
        }
      }
      else if (k == 3)
      {
        if (r > 0)
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::RedwoodTaiga);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), (int32)SelectRandom(COLD_BIOMES));
        }
      }
      else
      {
        newArr.Add(FIntVector2(i, j), (int32)SelectRandom(FROZEN_BIOMES));
      }
    }
  }

  return newArr;
}

// ██████╗ ██╗ ██████╗ ███╗   ███╗███████╗    ███████╗██████╗  ██████╗ ███████╗
// ██╔══██╗██║██╔═══██╗████╗ ████║██╔════╝    ██╔════╝██╔══██╗██╔════╝ ██╔════╝
// ██████╔╝██║██║   ██║██╔████╔██║█████╗      █████╗  ██║  ██║██║  ███╗█████╗  
// ██╔══██╗██║██║   ██║██║╚██╔╝██║██╔══╝      ██╔══╝  ██║  ██║██║   ██║██╔══╝  
// ██████╔╝██║╚██████╔╝██║ ╚═╝ ██║███████╗    ███████╗██████╔╝╚██████╔╝███████╗
// ╚═════╝ ╚═╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝    ╚══════╝╚═════╝  ╚═════╝ ╚══════╝


UBiomeGenerator::GenLayer* UBiomeGenerator::BiomeEdge::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::BiomeEdge::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (!ReplaceBiomeEdgeIfNecessary(parentArray, &newArr, i, j, k, (int32)EBiome::ExtremeHills,
                                       (int32)EBiome::SmallerExtremeHills) && !
        ReplaceBiomeEdgeIfNecessary(parentArray, &newArr, i, j, k, (int32)EBiome::MesaRock, (int32)EBiome::Mesa) && !
        ReplaceBiomeEdgeIfNecessary(parentArray, &newArr, i, j, k, (int32)EBiome::MesaClearRock,
                                    (int32)EBiome::Mesa) && !
        ReplaceBiomeEdgeIfNecessary(parentArray, &newArr, i, j, k, (int32)EBiome::RedwoodTaiga, (int32)EBiome::Taiga)
      )
      {
        int32 a = parentArray[FIntVector2(i + 1, j)];
        int32 b = parentArray[FIntVector2(i + 2, j + 1)];
        int32 c = parentArray[FIntVector2(i, j + 1)];
        int32 d = parentArray[FIntVector2(i + 1, j + 2)];

        if (k == (int32)EBiome::Desert)
        {
          if (a != (int32)EBiome::IcePlains && b != (int32)EBiome::IcePlains && c != (int32)EBiome::IcePlains && d != (
            int32)EBiome::IcePlains)
          {
            newArr.Add(FIntVector2(i, j), k);
          }
          else
          {
            newArr.Add(FIntVector2(i, j), (int32)EBiome::ExtremeHillsWithTrees);
          }
        }
        else if (k == (int32)EBiome::Swampland)
        {
          if (a != (int32)EBiome::Desert && b != (int32)EBiome::Desert && c != (int32)EBiome::Desert && d != (int32)
            EBiome::Desert && GetTemperatureCategory(a) != Cold &&
            GetTemperatureCategory(b) != Cold && GetTemperatureCategory(c) != Cold && GetTemperatureCategory(d) != Cold)
          {
            if (a != (int32)EBiome::Jungle && b != (int32)EBiome::Jungle && c != (int32)EBiome::Jungle && d != (int32)
              EBiome::Jungle)
            {
              newArr.Add(FIntVector2(i, j), k);
            }
            else
            {
              newArr.Add(FIntVector2(i, j), (int32)EBiome::Jungle);
            }
          }
          else
          {
            newArr.Add(FIntVector2(i, j), (int32)EBiome::Plains);
          }
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
    }
  }

  return newArr;
}

bool UBiomeGenerator::BiomeEdge::CanBeNeighbors(int32 biome1, int32 biome2)
{
  if (biome1 == biome2) return true;

  ETemperatureCategory temp1 = GetTemperatureCategory(biome1);
  ETemperatureCategory temp2 = GetTemperatureCategory(biome2);

  return (temp1 == temp2 || temp1 == Mild || temp2 == Mild);
}

bool UBiomeGenerator::BiomeEdge::ReplaceBiomeEdge(TMap<FIntVector2, int32> parentArray,
                                                  TMap<FIntVector2, int32>* newArr,
                                                  int32 i, int32 j, int32 targetBiome, int32 biomeToReplace,
                                                  int32 replacingBiome)
{
  if (targetBiome != biomeToReplace) return false;

  int32 a = parentArray[FIntVector2(i + 1, j)];
  int32 b = parentArray[FIntVector2(i + 2, j + 1)];
  int32 c = parentArray[FIntVector2(i, j + 1)];
  int32 d = parentArray[FIntVector2(i + 1, j + 2)];

  if (a == targetBiome && b == targetBiome && c == targetBiome && d == targetBiome)
  {
    newArr->FindOrAdd(FIntVector2(i, j), biomeToReplace);
  }
  else
  {
    newArr->FindOrAdd(FIntVector2(i, j), replacingBiome);
  }
  return true;
}

bool UBiomeGenerator::BiomeEdge::ReplaceBiomeEdgeIfNecessary(TMap<FIntVector2, int32> parentArray,
                                                             TMap<FIntVector2, int32>* newArray, int32 i, int32 j,
                                                             int32 targetBiome, int32 biomeToReplace,
                                                             int32 replacingBiome)
{
  if (targetBiome != biomeToReplace) return false;

  int32 a = parentArray[FIntVector2(i + 1, j)];
  int32 b = parentArray[FIntVector2(i + 2, j + 1)];
  int32 c = parentArray[FIntVector2(i, j + 1)];
  int32 d = parentArray[FIntVector2(i + 1, j + 2)];

  if (CanBeNeighbors(a, targetBiome) && CanBeNeighbors(b, targetBiome) && CanBeNeighbors(c, targetBiome) &&
    CanBeNeighbors(d, targetBiome))
  {
    newArray->FindOrAdd(FIntVector2(i, j), biomeToReplace);
  }
  else
  {
    newArray->FindOrAdd(FIntVector2(i, j), replacingBiome);
  }
  return true;
}

// ██████╗ ██╗██╗   ██╗███████╗██████╗     ██╗███╗   ██╗██╗████████╗
// ██╔══██╗██║██║   ██║██╔════╝██╔══██╗    ██║████╗  ██║██║╚══██╔══╝
// ██████╔╝██║██║   ██║█████╗  ██████╔╝    ██║██╔██╗ ██║██║   ██║   
// ██╔══██╗██║╚██╗ ██╔╝██╔══╝  ██╔══██╗    ██║██║╚██╗██║██║   ██║   
// ██║  ██║██║ ╚████╔╝ ███████╗██║  ██║    ██║██║ ╚████║██║   ██║   
// ╚═╝  ╚═╝╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝    ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝   


UBiomeGenerator::GenLayer* UBiomeGenerator::RiverInit::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::RiverInit::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(x, y, width, height);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      if (parentArray[FIntVector2(i, j)] > 0)
      {
        newArr.Add(FIntVector2(i, j), NextInt(299999) + 2);
      }
      else newArr.Add(FIntVector2(i, j), 0);
    }
  }

  return newArr;
}

// ██████╗ ██╗██╗   ██╗███████╗██████╗ 
// ██╔══██╗██║██║   ██║██╔════╝██╔══██╗
// ██████╔╝██║██║   ██║█████╗  ██████╔╝
// ██╔══██╗██║╚██╗ ██╔╝██╔══╝  ██╔══██╗
// ██║  ██║██║ ╚████╔╝ ███████╗██║  ██║
// ╚═╝  ╚═╝╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::RiverLayer::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::RiverLayer::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 a = RiverFilter(parentArray[FIntVector2(i + 1, j)]);
      int32 b = RiverFilter(parentArray[FIntVector2(i + 2, j + 1)]);
      int32 c = RiverFilter(parentArray[FIntVector2(i, j + 1)]);
      int32 d = RiverFilter(parentArray[FIntVector2(i + 1, j + 2)]);
      int32 k = RiverFilter(parentArray[FIntVector2(i + 1, j + 1)]);

      if (k == a && k == b && k == c && k == d)
      {
        newArr.Add(FIntVector2(i, j), -1);
      }
      else
      {
        newArr.Add(FIntVector2(i, j), (int32)EBiome::River);
      }
    }
  }

  return newArr;
}

int32 UBiomeGenerator::RiverLayer::RiverFilter(int32 value)
{
  return value >= 2 ? 2 + (value & 1) : value;
}

// ██╗  ██╗██╗██╗     ██╗     ███████╗
// ██║  ██║██║██║     ██║     ██╔════╝
// ███████║██║██║     ██║     ███████╗
// ██╔══██║██║██║     ██║     ╚════██║
// ██║  ██║██║███████╗███████╗███████║
// ╚═╝  ╚═╝╚═╝╚══════╝╚══════╝╚══════╝


UBiomeGenerator::GenLayer* UBiomeGenerator::Hills::Init(int64 baseSeed, GenLayer* parent, GenLayer* riverLayer)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;
  RiverLayer = riverLayer;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::Hills::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);
  TMap<FIntVector2, int32> riverArray = RiverLayer->GetInts(x, y, width, height);
  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      InitChunkSeed(x + i, y + j);
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];
      int32 l = riverArray[FIntVector2(i, j)];
      bool mutateHills = l >= 2 && ((l - 2) % 29 == 0);

      if (k != 0 && l >= 2 && (l - 2) % 29 == 1)
      {
        newArr.Add(FIntVector2(i, j), k);
      }
      else if (NextInt(3) != 0 && !mutateHills)
      {
        newArr.Add(FIntVector2(i, j), k);
      }
      else
      {
        int32 n = k;
        switch (k)
        {
        case (int32)EBiome::Desert: n = (int32)EBiome::DesertHills;
          break;
        case (int32)EBiome::Forest: n = (int32)EBiome::ForestHills;
          break;
        case (int32)EBiome::BirchForest: n = (int32)EBiome::BirchForestHills;
          break;
        case (int32)EBiome::RoofedForest: n = (int32)EBiome::Plains;
          break;
        case (int32)EBiome::Taiga: n = (int32)EBiome::TaigaHills;
          break;
        case (int32)EBiome::RedwoodTaiga: n = (int32)EBiome::RedwoodTaigaHills;
          break;
        case (int32)EBiome::TaigaCold: n = (int32)EBiome::TaigaColdHills;
          break;
        case (int32)EBiome::Plains: n = NextInt(3) == 0 ? (int32)EBiome::ForestHills : (int32)EBiome::Forest;
          break;
        case (int32)EBiome::IcePlains: n = (int32)EBiome::IceMountains;
          break;
        case (int32)EBiome::Jungle: n = (int32)EBiome::JungleHills;
          break;
        case (int32)EBiome::Ocean: n = (int32)EBiome::DeepOcean;
          break;
        case (int32)EBiome::ExtremeHills: n = (int32)EBiome::ExtremeHillsWithTrees;
          break;
        case (int32)EBiome::Savanna: n = (int32)EBiome::SavannaRock;
          break;
        default:
          if (BiomesEqualOrMesaPlateau(static_cast<EBiome>(k), EBiome::MesaRock)) n = (int32)EBiome::Mesa;
          else if (k == (int32)EBiome::DeepOcean && NextInt(3) == 0)
          {
            int32 c = NextInt(2);
            if (c == 0) n = (int32)EBiome::Plains;
            else n = (int32)EBiome::Forest;
          }
          break;
        }


        if (mutateHills && n != k)
        {
          n = k;
        }
        if (n == k)
        {
          newArr.Add(FIntVector2(i, j), k);
        }
        else
        {
          int32 a = parentArray[FIntVector2(i + 1, j)];
          int32 b = parentArray[FIntVector2(i + 2, j + 1)];
          int32 c = parentArray[FIntVector2(i, j + 1)];
          int32 d = parentArray[FIntVector2(i + 1, j + 2)];

          int32 sameNeighborCount = 0;
          if (BiomesEqualOrMesaPlateau(static_cast<EBiome>(a), static_cast<EBiome>(k))) sameNeighborCount++;
          if (BiomesEqualOrMesaPlateau(static_cast<EBiome>(b), static_cast<EBiome>(k))) sameNeighborCount++;
          if (BiomesEqualOrMesaPlateau(static_cast<EBiome>(c), static_cast<EBiome>(k))) sameNeighborCount++;
          if (BiomesEqualOrMesaPlateau(static_cast<EBiome>(d), static_cast<EBiome>(k))) sameNeighborCount++;

          newArr.Add(FIntVector2(i, j), sameNeighborCount >= 3 ? n : k);
        }
      }
    }
  }

  return newArr;
}


// ███████╗███╗   ███╗ ██████╗  ██████╗ ████████╗██╗  ██╗
// ██╔════╝████╗ ████║██╔═══██╗██╔═══██╗╚══██╔══╝██║  ██║
// ███████╗██╔████╔██║██║   ██║██║   ██║   ██║   ███████║
// ╚════██║██║╚██╔╝██║██║   ██║██║   ██║   ██║   ██╔══██║
// ███████║██║ ╚═╝ ██║╚██████╔╝╚██████╔╝   ██║   ██║  ██║
// ╚══════╝╚═╝     ╚═╝ ╚═════╝  ╚═════╝    ╚═╝   ╚═╝  ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::Smooth::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

TMap<FIntVector2, int32> UBiomeGenerator::Smooth::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);
  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 a = parentArray[FIntVector2(i, j + 1)];
      int32 b = parentArray[FIntVector2(i + 2, j + 1)];
      int32 c = parentArray[FIntVector2(i + 1, j)];
      int32 d = parentArray[FIntVector2(i + 1, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (a == b && c == d)
      {
        InitChunkSeed(x + i, y + j);
        k = NextInt(2) == 0 ? a : c;
      }
      else
      {
        if (a == b) k = a;
        if (c == d) k = c;
      }
      newArr.Add(FIntVector2(i, j), k);
    }
  }

  return newArr;
}

// ██████╗ ██╗██╗   ██╗███████╗██████╗     ███╗   ███╗██╗██╗  ██╗
// ██╔══██╗██║██║   ██║██╔════╝██╔══██╗    ████╗ ████║██║╚██╗██╔╝
// ██████╔╝██║██║   ██║█████╗  ██████╔╝    ██╔████╔██║██║ ╚███╔╝ 
// ██╔══██╗██║╚██╗ ██╔╝██╔══╝  ██╔══██╗    ██║╚██╔╝██║██║ ██╔██╗ 
// ██║  ██║██║ ╚████╔╝ ███████╗██║  ██║    ██║ ╚═╝ ██║██║██╔╝ ██╗
// ╚═╝  ╚═╝╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝    ╚═╝     ╚═╝╚═╝╚═╝  ╚═╝


UBiomeGenerator::GenLayer* UBiomeGenerator::RiverMix::Init(int64 baseSeed, GenLayer* parent, GenLayer* riverLayer)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;
  RiverLayer = riverLayer;

  return this;
}

void UBiomeGenerator::RiverMix::InitWorldGenSeed(int64 seed)
{
  GenLayer::InitWorldGenSeed(seed);
  if (RiverLayer != nullptr)
  {
    RiverLayer->InitWorldGenSeed(seed);
  }
}

TMap<FIntVector2, int32> UBiomeGenerator::RiverMix::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(x, y, width, height);
  TMap<FIntVector2, int32> riverArray = RiverLayer->GetInts(x, y, width, height);

  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 biome = parentArray[FIntVector2(i, j)];

      if (biome != (int32)EBiome::Ocean && biome != (int32)EBiome::DeepOcean)
      {
        if (riverArray[FIntVector2(i, j)] == (int32)EBiome::River)
        {
          if (biome == (int32)EBiome::IcePlains)
          {
            newArr.Add(FIntVector2(i, j), (int32)EBiome::FrozenRiver);
          }
          else if (biome != (int32)EBiome::MushroomIsland && biome != (int32)EBiome::MushroomIslandShore)
          {
            newArr.Add(FIntVector2(i, j), riverArray[FIntVector2(i, j)] & 255);
          }
          else
          {
            newArr.Add(FIntVector2(i, j), (int32)EBiome::MushroomIslandShore);
          }
        }
        else
        {
          newArr.Add(FIntVector2(i, j), biome);
        }
      }
      else
      {
        newArr.Add(FIntVector2(i, j), biome);
      }
    }
  }

  return newArr;
}

// ███████╗██╗  ██╗ ██████╗ ██████╗ ███████╗
// ██╔════╝██║  ██║██╔═══██╗██╔══██╗██╔════╝
// ███████╗███████║██║   ██║██████╔╝█████╗  
// ╚════██║██╔══██║██║   ██║██╔══██╗██╔══╝  
// ███████║██║  ██║╚██████╔╝██║  ██║███████╗
// ╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝


UBiomeGenerator::GenLayer* UBiomeGenerator::Shore::Init(int64 baseSeed, GenLayer* parent)
{
  GenLayer::Init(baseSeed, parent);
  Parent = parent;

  return this;
}

bool UBiomeGenerator::Shore::IsMesa(int32 biome)
{
  return biome == (int32)EBiome::Mesa || biome == (int32)EBiome::MesaRock || biome == (int32)EBiome::MesaClearRock;
}

bool UBiomeGenerator::Shore::IsJungleCompatible(int32 biome)
{
  return biome == (int32)EBiome::JungleEdge || biome == (int32)EBiome::Jungle || biome == (int32)EBiome::JungleHills ||
    biome == (int32)EBiome::Forest || biome == (int32)EBiome::Taiga ||
    IsOceanic(biome);
}

TMap<FIntVector2, int32> UBiomeGenerator::Shore::GetInts(int32 x, int32 y, int32 width, int32 height)
{
  TMap<FIntVector2, int32> newArr = TMap<FIntVector2, int32>();
  int32 parentX = x - 1;
  int32 parentY = y - 1;
  int32 parentWidth = width + 2;
  int32 parentHeight = height + 2;
  TMap<FIntVector2, int32> parentArray = Parent->GetInts(parentX, parentY, parentWidth, parentHeight);
  for (int32 j = 0; j < height; j++)
  {
    for (int32 i = 0; i < width; i++)
    {
      int32 a = parentArray[FIntVector2(i, j + 1)];
      int32 b = parentArray[FIntVector2(i + 2, j + 1)];
      int32 c = parentArray[FIntVector2(i + 1, j)];
      int32 d = parentArray[FIntVector2(i + 1, j + 2)];
      int32 k = parentArray[FIntVector2(i + 1, j + 1)];

      if (k == (int32)EBiome::MushroomIsland)
      {
        if (a != (int32)EBiome::Ocean && b != (int32)EBiome::Ocean && c != (int32)EBiome::Ocean && d != (int32)
          EBiome::Ocean)
        {
          newArr.Add(FIntVector2(i, j), k);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::MushroomIslandShore);
        }
      }
      else if (k == (int32)EBiome::Jungle || k == (int32)EBiome::JungleEdge || k == (int32)EBiome::JungleHills)
      {
        if (IsJungleCompatible(a) && IsJungleCompatible(b) && IsJungleCompatible(c) && IsJungleCompatible(d))
        {
          if (IsOceanic(a) || IsOceanic(b) || IsOceanic(c) || IsOceanic(d))
          {
            newArr.Add(FIntVector2(i, j), (int32)EBiome::Beach);
          }
          else
          {
            newArr.Add(FIntVector2(i, j), k);
          }
        }
        else
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::JungleEdge);
        }
      }
      else if (k == (int32)EBiome::ExtremeHills || k == (int32)EBiome::ExtremeHillsWithTrees || k == (int32)
        EBiome::SmallerExtremeHills)
      {
        if (IsOceanic(a) || IsOceanic(b) || IsOceanic(c) || IsOceanic(d))
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::StoneBeach);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
      else if (IsSnowy(k))
      {
        if (!IsOceanic(k) && (IsOceanic(a) || IsOceanic(b) || IsOceanic(c) || IsOceanic(d)))
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::ColdBeach);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
      else if (k == (int32)EBiome::Mesa || k == (int32)EBiome::MesaRock)
      {
        if ((!IsOceanic(a) && !IsOceanic(b) && !IsOceanic(c) && !IsOceanic(d)) && (!IsMesa(a) || !IsMesa(b) || !
          IsMesa(c) || !IsMesa(d)))
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::Desert);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
      else if (k != (int32)EBiome::Ocean && k != (int32)EBiome::DeepOcean && k != (int32)EBiome::River && k != (int32)
        EBiome::Swampland)
      {
        if (IsOceanic(a) || IsOceanic(b) || IsOceanic(c) || IsOceanic(d))
        {
          newArr.Add(FIntVector2(i, j), (int32)EBiome::Beach);
        }
        else
        {
          newArr.Add(FIntVector2(i, j), k);
        }
      }
      else
      {
        newArr.Add(FIntVector2(i, j), k);
      }
    }
  }

  return newArr;
}