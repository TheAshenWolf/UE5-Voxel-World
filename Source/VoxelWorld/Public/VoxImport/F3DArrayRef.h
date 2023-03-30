#pragma once
#include "FVoxelRef.h"
#include "F3DArrayRef.generated.h"

USTRUCT()
struct F3DArrayRef
{
  GENERATED_BODY()
private:
  /// <summary>
  /// X size of the array
  /// </summary>
  UPROPERTY() int32 XSize;
  /// <summary>
  /// Y size of the array
  /// </summary>
  UPROPERTY() int32 YSize;
  /// <summary>
  /// Z size of the array
  /// </summary>
  UPROPERTY() int32 ZSize;

private:
  /// <summary>
  /// The actual array holding the data
  /// </summary>
  UPROPERTY() TMap<FIntVector, FVoxelRef> Data;

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
  FVoxelRef Get(int32 x, int32 y, int32 z);

  /// <summary>
  /// Overload for Get taking in FIntVector
  /// </summary>
  FVoxelRef Get(FIntVector pos);

  
  void Set(FIntVector position, FVoxelRef value, bool overrideExisting = true);
  
  /// <summary>
  /// Sets an element at the given index or does nothing if the index is out of bounds
  /// </summary>
  /// <param name="x">X coordinate</param>
  /// <param name="y">Y coordinate</param>
  /// <param name="z">Z coordinate</param>
  /// <param name="value">New value at the given index</param>
  void Set(int32 x, int32 y, int32 z, FVoxelRef value, bool overrideExisting = true);

  /// <summary>
  /// Constructor to initialize the array with a given size
  /// </summary>
  /// <param name="x">Size along the X axis</param>
  /// <param name="y">Size along the Y axis</param>
  /// <param name="z">Size along the Z axis</param>
  F3DArrayRef(int32 x, int32 y, int32 z);

  /// <summary>
  /// Default constructor; Do not use
  /// </summary>
  F3DArrayRef();

  void Append(F3DArrayRef other);

  bool Contains(FIntVector position);

  void Empty();
};
