#include "VoxImport/F3DArrayRef.h"
#include "VoxImport/FVoxelRef.h"

F3DArrayRef::F3DArrayRef(int32 xSize, int32 ySize, int32 zSize)
{
  XSize = xSize;
  YSize = ySize;
  ZSize = zSize;

  Data = TMap<FIntVector, FVoxelRef>();
  for (int32 x = 0; x < XSize; x++)
  {
    for (int32 y = 0; y < YSize; y++)
    {
      for (int32 z = 0; z < ZSize; z++)
      {
        FIntVector pos = FIntVector(x, y, z);
        Data.Add(pos, FVoxelRef());
      }
    }
  }
}


FVoxelRef F3DArrayRef::Get(int32 x, int32 y, int32 z)
{
  if (!Data.Contains(FIntVector(x, y, z)))
  {
    return FVoxelRef();
  }

  return Data[FIntVector(x, y, z)];
}

FVoxelRef F3DArrayRef::Get(FIntVector pos)
{
  return Get(pos.X, pos.Y, pos.Z);
}


void F3DArrayRef::Set(FIntVector position, FVoxelRef value, bool overrideExisting)
{
  if (Data.Contains(position) && overrideExisting)
  {
    Data[position] = value;
  }
  else
  {
    Data.Add(position, value);
  }
}

void F3DArrayRef::Set(int32 x, int32 y, int32 z, FVoxelRef value, bool overrideExisting)
{
  Set(FIntVector(x, y, z), value, overrideExisting);
}


int32 F3DArrayRef::Length()
{
  return XSize * YSize * ZSize;
}


FIntVector F3DArrayRef::Length3D()
{
  return FIntVector(XSize, YSize, ZSize);
}


F3DArrayRef::F3DArrayRef(): XSize(0), YSize(0), ZSize(0)
{
  // No initialization
}

void F3DArrayRef::Append(F3DArrayRef other)
{
  for (auto& pair : other.Data)
  {
    Set(pair.Key, pair.Value);
  }
}

bool F3DArrayRef::Contains(FIntVector position)
{
  return Data.Contains(position);
}

void F3DArrayRef::Empty()
{
  Data.Empty();
  XSize = 0;
  YSize = 0;
  ZSize = 0;
}
