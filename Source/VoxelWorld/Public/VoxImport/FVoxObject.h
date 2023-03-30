#pragma once
#include "F3DArray.h"
#include "FVoxel.h"

#include "FVoxObject.generated.h"

constexpr int PALETTE_LENGTH = 255;

USTRUCT()
struct FVoxObject
{
  GENERATED_BODY()
  
  /// <summary>
  /// Size of the Voxel object
  /// </summary>
  int32 SizeX, SizeY, SizeZ;
  /// <summary>
  /// 3D array of voxels
  /// </summary>
  F3DArray Voxels;
  /// <summary>
  /// Constructor for empty voxel object of given size
  /// </summary>
  FVoxObject(int32 sizeX, int32 sizeY, int32 sizeZ);
  /// <summary>
  /// Fills the whole object with black voxels
  /// </summary>
  void Fill();
  /// <summary>
  /// Counts all the non-empty voxels within the object
  /// </summary>
  /// <returns>Amount of voxels which are not empty</returns>
  int32 CountFilledVoxels();
  /// <summary>
  /// Gets all the non-empty voxels within the object
  /// </summary>
  /// <returns>An Array of filled voxels</returns>
  TArray<FVoxel> GetFilledVoxels();

  /// <summary>
  /// Default constructor. Do not use.
  /// </summary>
  FVoxObject();
};
