#include "VoxImport/F3DArray.h"

F3DArray::F3DArray(int32 xSize, int32 ySize, int32 zSize)
{
  XSize = xSize;
  YSize = ySize;
  ZSize = zSize;
  
  Data = TMap<FIntVector, FVoxel>();
  for (int32 x = 0; x < XSize; x++)
  {
    for (int32 y = 0; y < YSize; y++)
    {
      for (int32 z = 0; z < ZSize; z++)
      {
        FIntVector pos = FIntVector(x, y, z);
        Data.Add(pos, FVoxel());
      }
    }
  }
}


FVoxel F3DArray::Get(int32 x, int32 y, int32 z)
{
  if (!Data.Contains(FIntVector(x, y,z )))
  {
    return FVoxel();
  }

  return Data[FIntVector(x, y, z)];
}

FVoxel F3DArray::Get(FIntVector pos)
{
  return Get(pos.X, pos.Y, pos.Z);
}


void F3DArray::Set(FIntVector position, FVoxel value)
{
  if (Data.Contains(position))
  {
    Data[position] = value;
  }
  else
  {
    Data.Add(position, value);
  }
  
  
}

void F3DArray::Set(int32 x, int32 y, int32 z, FVoxel value)
{
  Set(FIntVector(x, y, z), value);
}


int32 F3DArray::Length()
{
  return XSize * YSize * ZSize;
}


FIntVector F3DArray::Length3D()
{
  return FIntVector(XSize, YSize, ZSize);
}


F3DArray::F3DArray(): XSize(0), YSize(0), ZSize(0)
{
  // No initialization
}

bool F3DArray::FindVoxelByValue(int32 value, FIntVector& out)
{
  for (TTuple<FIntVector, FVoxel>& voxel : Data)
  {
    if (voxel.Get<1>().Color == value)
    {
      out = voxel.Get<0>();
      return true;
    }
  }

  return false;
}
