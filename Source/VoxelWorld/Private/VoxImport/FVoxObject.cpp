#include "VoxImport/FVoxObject.h"

FVoxObject::FVoxObject(int32 sizeX, int32 sizeY, int32 sizeZ)
{
  SizeX = sizeX;
  SizeY = sizeY;
  SizeZ = sizeZ;

  Voxels = F3DArray(sizeX, sizeY, sizeZ);
  for (int32 x = 0; x < sizeX; x++)
  {
    for (int32 y = 0; y < sizeY; y++)
    {
      for (int32 z = 0; z < sizeZ; z++)
      {
        Voxels.Set(x, y, z, FVoxel(x, y, z));
      }
    }
  }  
}

void FVoxObject::Fill()
{
  for (int32 x = 0; x < SizeX; x++)
  {
    for (int32 y = 0; y < SizeY; y++)
    {
      for (int32 z = 0; z < SizeZ; z++)
      {
        Voxels.Set(x, y, z, FVoxel(x, y, z));
      }
    }
  }
}

int32 FVoxObject::CountFilledVoxels()
{
  int32 count = 0;
  for (int32 x = 0; x < SizeX; x++)
  {
    for (int32 y = 0; y < SizeY; y++)
    {
      for (int32 z = 0; z < SizeZ; z++)
      {
        if (!Voxels.Get(x, y,z).IsEmpty)
        {
          count++;
        }
      }
    }
  }

  return count;
}

TArray<FVoxel> FVoxObject::GetFilledVoxels()
{
  TArray<FVoxel> voxels = TArray<FVoxel>();
  for (int32 x = 0; x < SizeX; x++)
  {
    for (int32 y = 0; y < SizeY; y++)
    {
      for (int32 z = 0; z < SizeZ; z++)
      {
        FVoxel voxel = Voxels.Get(x, y,z);
        if (!voxel.IsEmpty)
        {
          voxels.Add(voxel);
        }
      }
    }
  }

  return voxels;
}

FVoxObject::FVoxObject(): SizeX(0), SizeY(0), SizeZ(0)
{
  // No implementation
}
