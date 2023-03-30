#pragma once
#include "FVoxel.h"


struct F3DArray
{
private:
  /// <summary>
  /// X size of the array
  /// </summary>
  int32 XSize;
  /// <summary>
  /// Y size of the array
  /// </summary>
  int32 YSize;
  /// <summary>
  /// Z size of the array
  /// </summary>
  int32 ZSize;

private:
  /// <summary>
  /// The actual array holding the data
  /// </summary>
  TMap<FIntVector, FVoxel> Data;

public:
  
  /// <summary>
  /// Returns the lenght of the array in it's flat form
  /// </summary>
  /// <returns>Length of the array</returns>
  int32 Length();

  /// <summary>
  /// Returns the length of the array in it's 3D form
  /// </summary>
  /// <returns>Length of the array in a 3D space</returns>
  FIntVector Length3D();

  /// <summary>
  /// Returns an element at the given index or <see cref="nullptr"/> if the index is out of bounds
  /// </summary>
  /// <param name="x">X coordinate</param>
  /// <param name="y">Y coordinate</param>
  /// <param name="z">Z coordinate</param>
  /// <returns>Item in the array or nullptr in case of an invalid index</returns>
  FVoxel Get(int32 x, int32 y, int32 z);

  /// <summary>
  /// Overload for Get taking in FIntVector
  /// </summary>
  FVoxel Get(FIntVector pos);
  void Set(FIntVector position, FVoxel value);

  /// <summary>
  /// Overload using flat array index
  /// </summary>
  /// <param name="index">index of the block in 1D array</param>
  /// <returns>Voxel on the position or empty voxel</returns>
  FVoxel Get(int32 index);
  
  /// <summary>
  /// Sets an element at the given index or does nothing if the index is out of bounds
  /// </summary>
  /// <param name="x">X coordinate</param>
  /// <param name="y">Y coordinate</param>
  /// <param name="z">Z coordinate</param>
  /// <param name="value">New value at the given index</param>
  void Set(int32 x, int32 y, int32 z, FVoxel value);

  /// <summary>
  /// Constructor to initialize the array with a given size
  /// </summary>
  /// <param name="x">Size along the X axis</param>
  /// <param name="y">Size along the Y axis</param>
  /// <param name="z">Size along the Z axis</param>
  F3DArray(int32 x, int32 y, int32 z);

  /// <summary>
  /// Default constructor; Do not use
  /// </summary>
  F3DArray();

  bool FindVoxelByValue(int32 value, FIntVector& out);
};
